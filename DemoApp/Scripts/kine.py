import dk
import math


def createRigidBody(mass, shape, transform=dk.NSTransform(), name=''):
    if mass == 0.0:
        localInertia = dk.Vector3(0, 0, 0)
    else:
        localInertia = shape.calculateLocalInertia(mass)

    rb = dk.RigidBody(shape, mass, localInertia)
    rb.setWorldTransform(transform)
    rb.name = name
    return rb


class CameraInfo:
    def __init__(self, fov=math.radians(80.0), near=10.0, far=100.0, aspect=1.0, target=dk.Vector3(0, 0, 0)):
        self.fov = fov
        self.near = near
        self.far = far
        self.aspect = aspect
        self.target = target


class Frame(dk.ui.View):
    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)
        self.model = None
        self.resourcePool = None
        self.scene = None

    def onLoaded(self):
        super().onLoaded()
        self.backgroundColor = dk.Color(0.25, 0.25, 0.25)

        self.infoLabel = dk.ui.Label(' Left-Click: camera move, Right-Click: Shoot box (Second touch on mobile)')
        self.infoLabel.fontAttributes = dk.ui.font.attributes(14, outline=2)
        self.infoLabel.align = dk.ui.label.ALIGN_LEFT
        self.infoLabel.setBlendState(dk.blendstate.defaultAlpha)
        self.infoLabel.backgroundColor = dk.color.clear
        self.infoLabel.textColor = dk.color.white
        self.infoLabel.outlineColor = dk.color.black
        self.infoLabel.enabled = False
        self.addChild(self.infoLabel)

        # 슈팅 박스
        self.shootingShapes = (dk.BoxShape(0.85, 0.85, 0.85),
                               dk.BoxShape(1.0, 1.0, 1.0),
                               dk.BoxShape(2.0, 1.0, 2.0),
                               dk.CylinderShape(1.0, 1.0, 1.0),
                               dk.CylinderShape(1.5, 0.5, 1.5),
                               dk.SphereShape(1.0))

        self.resourcePool = dk.ResourcePool()
        self.resourcePool.addSearchPath(dk.appInstance().resourceDir + "/dao")

        # 씬 생성
        self.model = self.resourcePool.loadResource('dao.DKMODEL')
        animaiton = self.resourcePool.loadResource('dao.DKANIMATION')
        animControl = animaiton.createLoopController()
        animControl.play()
        self.model.setAnimation(animControl)

        self.scene = dk.DynamicsScene()
        self.scene.drawMode = dk.scene.DRAW_COLLISION_SHAPES | dk.scene.DRAW_MESHES
        self.scene.ambientColor = dk.Color(0.45, 0.45, 0.45)
        self.scene.lights.append(dk.light.directional(dk.Vector3(0, -1, 0), dk.Color(1, 1, 1)))
        self.scene.updateLights()
        self.scene.addObject(self.model)

        # 바닥 객체 생성
        groundShape = dk.StaticPlaneShape(dk.Vector3(0, 1, 0), 0)
        groundObject = createRigidBody(0, groundShape, dk.Vector3(0, 0, 0), 'groundObject')
        self.scene.addObject(groundObject)

        # 카메라 초기화.
        self.camera = dk.Camera()
        self.cameraInfo = CameraInfo(fov=math.radians(80),
                                     near=1.0,
                                     far=1000.0,
                                     aspect=1.0,
                                     target=dk.Vector3(0, 5, 0))
        pos = dk.Vector3(0, 10, -20)
        up = dk.Vector3(0, 1, 0)
        self.camera.setView(pos, self.cameraInfo.target - pos, up)


    def onUnload(self):
        super().onUnload()

        self.model = None
        self.resourcePool = None
        self.infoLabel = None
        self.shootingShapes = None

        self.scene = None

    def updatePerspective(self):
        w, h = self.contentResolution
        self.cameraInfo.aspect = w / h
        self.camera.setPerspective(self.cameraInfo.fov, self.cameraInfo.aspect, self.cameraInfo.near,
                                   self.cameraInfo.far)

    def layout(self):
        super().layout()
        bounds = self.bounds()
        self.infoLabel.frame = dk.Rect(0, 0, bounds.width, 50)

        self.updatePerspective()

    def shootBox(self, start, destination, speed=30):

        n = dk.random() % len(self.shootingShapes)

        start = dk.Vector3(start)
        if start.y < 1:
            start.y = 1

        box = createRigidBody(50.0, self.shootingShapes[n], start, name='a box')
        box.setLinearFactor(dk.Vector3(1, 1, 1))
        linVel = destination - start
        linVel.normalize()
        linVel *= speed

        box.setLinearVelocity(linVel)
        box.setAngularVelocity(dk.Vector3(0, 0, 0))

        self.scene.addObject(box)

    def onUpdate(self, delta, tick, date):
        self.redraw()
        self.scene.update(delta, tick)

    def onRender(self, renderer):
        super().onRender(renderer)
        def objColorCb(obj, *args):
            if obj.isKinematic():
                return None
            return dk.Color(1, 0.2, 0.2)

        renderer.renderScene(self.scene, self.camera, 0, objectColorCallback=objColorCb)

    def onMouseDown(self, deviceId, buttonId, pos):
        super().onMouseDown(deviceId, buttonId, pos)
        if deviceId == 0:
            self.captureMouse(deviceId)

    def onMouseUp(self, deviceId, buttonId, pos):
        super().onMouseUp(deviceId, buttonId, pos)
        if deviceId == 0:
            if self.isMouseCapturedBySelf(deviceId):
                self.releaseMouse(deviceId)
        if deviceId == 1 or buttonId == 1:
            x = (pos.x / self.contentScale[0]) * 2.0 - 1.0
            y = (pos.y / self.contentScale[1]) * 2.0 - 1.0

            viewProjInv = self.camera.viewMatrix() * self.camera.projectionMatrix()
            viewProjInv.inverse()

            rayBegin = dk.Vector3(x, y, -1.0)
            rayEnd = dk.Vector3(x, y, 1.0)

            rayBegin.transform(viewProjInv)
            rayEnd.transform(viewProjInv)

            # target = self.cameraInfo.target
            self.shootBox(rayBegin, rayEnd)
            print('shooting Box!!')

    def onMouseMove(self, deviceId, pos, delta):
        super().onMouseMove(deviceId, pos, delta)
        if deviceId == 0:
            if self.isMouseCapturedBySelf(deviceId):
                dir = self.camera.direction()
                # up = self.camera.up()
                up = dk.Vector3(0, 1, 0)
                left = dir.cross(up)

                dX = delta.x * 0.01
                dY = delta.y * 0.01

                qY = dk.Quaternion(up, -dX)
                qX = dk.Quaternion(left, dY)
                rot = qX * qY

                pos = self.camera.position() - self.cameraInfo.target
                pos.transform(rot)
                pos += self.cameraInfo.target
                minY = self.cameraInfo.target.y + self.cameraInfo.near + 2
                pos.y = max(pos.y, self.cameraInfo.target.y)
                self.camera.setView(pos, self.cameraInfo.target - pos, up)

    def onMouseWheel(self, deviceId, pos, delta):
        super().onMouseWheel(deviceId, pos, delta)
        if deviceId == 0:
            pos = self.camera.position()
            dir = self.cameraInfo.target - pos

            d1 = dir.length()
            d2 = d1 + delta.y

            d2 = max(d2, 25)
            if d1 != d2:
                up = self.camera.up()
                p = pos - self.cameraInfo.target
                p.normalize()
                p *= d2
                p += self.cameraInfo.target

                self.camera.setView(p, self.cameraInfo.target - p, up)
