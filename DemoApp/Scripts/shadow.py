import dk
import math
import array

SCENE_PASS_DEFAULT = 0
SCENE_PASS_SHADOW_MAP = 1
SCENE_PASS_SHADOWED = 2
SCENE_PASS_SHADOWED_MULTISAMPLE = 3

SHADOW_MAP_UNIFORM = 0
SHADOW_MAP_LISPSM = 1

class CameraInfo:
    fov = math.radians(60)
    near = 100
    far = 100000
    aspect = 1.0
    target = dk.Vector3(0, 0, 0)


class ShadowMapInfo:
    type = SHADOW_MAP_UNIFORM
    resolution = 0
    bvector = True
    multisample = False

    biasMatrix = dk.Matrix4(0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0,
                            0.5, 0.5, 0.5, 1.0)

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.renderer = None
        self.camera = dk.Camera()
        self.aabb = None

    def __repr__(self):
        return 'type:{}, resolution:{}, b-vector:{}, multi-sample:{}'.format(self.type, self.resolution, self.bvector, self.multisample)


class Frame(dk.ui.View):
    def __init__(self, *args, **kw):
        super().__init__(*args, **kw)

        self.cameraInfo = CameraInfo()
        self.cameraInfo.target = dk.Vector3(0, 1000, 0)

        self.camera = dk.Camera()
        self.inspectCamera = dk.Camera()

        pos = dk.Vector3(0, 1000, -1700)
        self.camera.setView(pos, self.cameraInfo.target - pos, dk.Vector3(0, 1, 0))
        self.inspectCamera.setView(pos, self.cameraInfo.target - pos, dk.Vector3(0, 1, 0))

        self.backgroundColor = dk.Color(0.85, 0.85, 1.0)
        self.inspectMode = False
        self.shadowMapInfo = ShadowMapInfo()
        self.shadowMapInfo.type = SHADOW_MAP_LISPSM

    def updatePerspective(self):
        w,h = self.contentResolution
        self.cameraInfo.aspect = w/h
        self.camera.setPerspective(self.cameraInfo.fov, self.cameraInfo.aspect, self.cameraInfo.near, self.cameraInfo.far)
        self.inspectCamera.setPerspective(self.cameraInfo.fov, self.cameraInfo.aspect, self.cameraInfo.near, self.cameraInfo.far)

    # def updateView(self):
    #     pos = self.camera.position()
    #     #up = self.camera.up()
    #     up = dk.Vector3(0, 1, 0)
    #     dir = self.cameraInfo.target - pos
    #     self.camera.setView(pos, dir, up)

    def onLoaded(self):
        super().onLoaded()

        bounds = self.bounds()

        # slider for light direction
        sliderX = dk.ui.Slider(0, range=(-1.5, 1.5), frame=dk.Rect(0, 100, 200, 40))
        sliderZ = dk.ui.Slider(0, range=(-1.5, 1.5), frame=dk.Rect(0, 150, 40, 200))
        sliderZ.vertical = True
        sliderX.vectorIndex = 0
        sliderZ.vectorIndex = 2
        sliderX.addTarget(self, self.onSliderValueChanged)
        sliderZ.addTarget(self, self.onSliderValueChanged)
        self.addChild(sliderX)
        self.addChild(sliderZ)

        shadowConfigWindow = dk.ui.TitledView('Config')
        origin = shadowConfigWindow.contentBounds().origin

        resWidth = 100
        typeWidth = 120
        itemHeight = 30

        resolutions = (0, 512)
        r = resolutions[-1]
        maxTexSize = dk.texture.maxTextureSize()
        while r < maxTexSize:
            r = r * 2
            resolutions = resolutions + (r,)

        # resolution radio-button rect
        resRect = dk.Rect(origin[0], origin[1], resWidth, len(resolutions) * itemHeight)

        configHeight = max(resRect.height, itemHeight*5)
        resRect.y = origin[1] + configHeight - resRect.height

        # shadow-type radio-button rect
        typeRect = dk.Rect(resRect.x + resRect.width, 0, typeWidth, itemHeight*2)
        typeRect.y = origin[1] + configHeight - typeRect.height

        # shadow-body-vector checkbox rect
        bvRect = dk.Rect(resRect.x + resRect.width, 0, typeWidth, itemHeight)
        bvRect.y = typeRect.y - bvRect.height

        # multi-sample checkbox rect
        msRect = dk.Rect(resRect.x + resRect.width, 0, typeWidth, itemHeight)
        msRect.y = bvRect.y - msRect.height

        # inspect-mode checkbox rect
        imRect = dk.Rect(resRect.x + resRect.width, 0, typeWidth, itemHeight)
        imRect.y = msRect.y - msRect.height

        frame = shadowConfigWindow.frameForContentFrame(dk.Rect(0, 0, resWidth+typeWidth, configHeight ))
        frame.origin = bounds.width - frame.width, bounds.height - frame.height
        shadowConfigWindow.frame = frame

        for btn, val in zip(
                dk.ui.radio.addItems(shadowConfigWindow, resolutions, resRect, columns=1),
                resolutions):
            btn.value = val
            btn.addTarget(self, self.onShadowResolutionChanged)
            btn.backgroundColor = dk.color.clear
            btn.setBlendState(dk.blendstate.defaultAlpha)

        for btn, val in zip(
                dk.ui.radio.addItems(shadowConfigWindow, ('Uniform', 'LiSPSM'), typeRect, columns=1),
                (SHADOW_MAP_UNIFORM, SHADOW_MAP_LISPSM)):
            btn.value = val
            if val == self.shadowMapInfo.type:
                btn.setSelected()
            btn.addTarget(self, self.onShadowTypeChanged)
            btn.backgroundColor = dk.color.clear
            btn.setBlendState(dk.blendstate.defaultAlpha)

        cb = dk.ui.Checkbox('B-Vector', value=True, frame=bvRect)
        cb.addTarget(self, self.onShadowBVectorValueChanged)
        cb.backgroundColor = dk.color.clear
        cb.setBlendState(dk.blendstate.defaultAlpha)
        shadowConfigWindow.addChild(cb)
        cb = dk.ui.Checkbox('Multi-Sample', value=False, frame=msRect)
        cb.addTarget(self, self.onShadowMultiSampleValueChanged)
        cb.backgroundColor = dk.color.clear
        cb.setBlendState(dk.blendstate.defaultAlpha)
        shadowConfigWindow.addChild(cb)
        cb = dk.ui.Checkbox('Inspect', value=False, frame=imRect)
        cb.addTarget(self, self.onShadowInspdeModeValueChanged)
        cb.backgroundColor = dk.color.clear
        cb.setBlendState(dk.blendstate.defaultAlpha)
        shadowConfigWindow.addChild(cb)
        shadowConfigWindow.backgroundColor = dk.Color(1.0, 1.0, 1.0, 0.9)
        shadowConfigWindow.setBlendState(dk.blendstate.defaultAlpha)
        self.addChild(shadowConfigWindow)

        # info labels
        self.infoLabel = dk.ui.Label()
        self.infoLabel.fontAttributes = dk.ui.font.attributes(18)
        self.infoLabel.align = dk.ui.label.ALIGN_LEFT
        self.infoLabel.backgroundColor = dk.Color(0.0, 0.0, 0.0, 0.3)
        self.infoLabel.textColor = dk.Color(1.0, 1.0, 1.0, 1.0)
        self.infoLabel.outlineColor = dk.Color(0.0, 0.0, 0.0, 1.0)
        self.infoLabel.setBlendState(dk.blendstate.defaultAlpha)
        self.infoLabel.enabled = False
        self.addChild(self.infoLabel)

        self.screen().postOperation(self.layout, ())

        # load model
        resourcePool = dk.ResourcePool()
        resourcePool.addSearchPath(dk.appInstance().resourceDir + '/desert')

        self.scene = dk.Scene()
        self.model = resourcePool.loadResource('desert.DKMODEL')
        self.scene.addObject(self.model)
        self.scene.ambientColor = dk.Color(0.1, 0.1, 0.1)
        self.scene.lights.append(dk.light.directional(dk.Vector3(0, -1, 0), dk.Color(1, 1, 1)))
        self.scene.updateLights()

        # calculate aabb
        def calcAABB(model, aabb):
            if isinstance(model, dk.Mesh):
                aabbMin, aabbMax = model.aabb()
                if aabbMin.x < aabb.min.x:  aabb.min.x = aabbMin.x
                if aabbMin.y < aabb.min.y:  aabb.min.y = aabbMin.y
                if aabbMin.z < aabb.min.z:  aabb.min.z = aabbMin.z
                if aabbMax.x > aabb.max.x:  aabb.max.x = aabbMax.x
                if aabbMax.y > aabb.max.y:  aabb.max.y = aabbMax.y
                if aabbMax.z > aabb.max.z:  aabb.max.z = aabbMax.z
            for c in model.children():
                calcAABB(c, aabb)

        aabb = dk.shadow.AABB(dk.Vector3(0, 0, 0), dk.Vector3(0, 0, 0))
        print('calculating scene AABB...')
        calcAABB(self.model, aabb)
        print('AABB: ', aabb)
        self.shadowMapInfo.aabb = aabb

        # generate box polygon mesh
        decl = dk.geometry.VertexBufferDecl()
        decl.add(dk.geometry.STREAM_ID.POSITION, 'position', dk.geometry.STREAM_TYPE.FLOAT3)
        data = array.array('f')
        data.extend(( 1.0,  1.0, -1.0))   # near right top
        data.extend(( 1.0, -1.0, -1.0))   # near right bottom
        data.extend((-1.0, -1.0, -1.0))   # near left bottom
        data.extend((-1.0,  1.0, -1.0))   # near left top
        data.extend(( 1.0,  1.0,  1.0))   # far right top
        data.extend(( 1.0, -1.0,  1.0))   # far right bottom
        data.extend((-1.0, -1.0,  1.0))   # far left bottom
        data.extend((-1.0,  1.0,  1.0))   # far left top

        faceIndices = (2,1,3,0,7,4,6,5,        # 2,1,3 / 3,1,0 / 3,0,7 / 7,0,4 / 7,4,6 / 6,4,5
                   5,4,
                   4,0,5,1,6,2,7,3)        # 4,0,5 / 5,0,1 / 5,1,6 / 6,1,2 / 6,2,7 / 7,2,3

        lineIndices = (0,1,1,2,2,3,3,0,0,4,4,5,5,1,3,7,7,6,6,2,6,5,7,4)

        vb = dk.geometry.createVertexBuffer(decl, data, 8)
        faceIb = dk.geometry.createIndexBuffer(faceIndices, dk.geometry.PRIMITIVE_TRIANGLE_STRIP)
        lineIb = dk.geometry.createIndexBuffer(lineIndices, dk.geometry.PRIMITIVE_LINES)

        inspectMaterial = resourcePool.loadResource('inspect.DKMATERIAL')
        inspectFaceMesh = dk.StaticMesh()
        # inspectFaceMesh.drawFace = dk.mesh.DRAW_FACE_CCW
        inspectFaceMesh.drawFace = dk.mesh.DRAW_FACE_BOTH
        inspectFaceMesh.setMaterial(inspectMaterial)
        inspectFaceMesh.addVertexBuffer(vb)
        inspectFaceMesh.setIndexBuffer(faceIb)

        inspectLineMesh = dk.StaticMesh()
        inspectLineMesh.drawFace = dk.mesh.DRAW_FACE_BOTH
        inspectLineMesh.setMaterial(inspectMaterial)
        inspectLineMesh.addVertexBuffer(vb)
        inspectLineMesh.setIndexBuffer(lineIb)

        # inspect mode items.
        self.inspectScene = dk.Scene()
        self.cameraFrustumFaceMesh = inspectFaceMesh.clone()
        self.shadowFrustumFaceMesh = inspectFaceMesh.clone()
        self.cameraFrustumLineMesh = inspectLineMesh.clone()
        self.shadowFrustumLineMesh = inspectLineMesh.clone()

        self.cameraFrustumFaceMesh.setMaterialProperty('color', floatings=dk.Color(1,0,0,0.1).tuple)
        self.shadowFrustumFaceMesh.setMaterialProperty('color', floatings=dk.Color(0,0,1,0.1).tuple)
        self.cameraFrustumLineMesh.setMaterialProperty('color', floatings=dk.Color(1,1,1).tuple)
        self.shadowFrustumLineMesh.setMaterialProperty('color', floatings=dk.Color(1,1,1).tuple)

        self.inspectScene.addObject(self.cameraFrustumFaceMesh)
        self.inspectScene.addObject(self.shadowFrustumFaceMesh)
        self.inspectScene.addObject(self.cameraFrustumLineMesh)
        self.inspectScene.addObject(self.shadowFrustumLineMesh)


    def onUnload(self):
        super().onUnload()

        self.scene = None
        self.model = None
        self.shadowMapInfo = None

        self.inspectScene = None
        self.cameraFrustumFaceMesh = None
        self.shadowFrustumFaceMesh = None
        self.cameraFrustumLineMesh = None
        self.shadowFrustumLineMesh = None

    def layout(self):
        super().layout()
        self.updatePerspective()

        if self.infoLabel.font:
            width = self.contentResolution[0]
            height = self.infoLabel.font.lineHeight()
            frame = self.convertPixelToLocal(dk.Rect(0, 0, width, height+15))
            self.infoLabel.frame = frame

    def onUpdate(self, delta, tick, date):
        super().onUpdate(delta, tick, date)

        shadowCalcElapsed = 0.0

        if self.shadowMapInfo.resolution != 0:
            light = self.scene.lights[0]
            aabb = self.shadowMapInfo.aabb

            timer = dk.Timer()
            if self.shadowMapInfo.type == SHADOW_MAP_LISPSM:
                v, p = dk.shadow.lispSMMatrices(self.camera, light.direction(), aabb,
                                                useBodyVector=self.shadowMapInfo.bvector)
            else:
                v, p = dk.shadow.uniformSMMatrices(self.camera, light.direction(), aabb,
                                                   useBodyVector=self.shadowMapInfo.bvector)
            self.shadowMapInfo.camera.setViewProjectionMatrix(v, p)
            shadowCalcElapsed = timer.elapsed()

            texSize = self.shadowMapInfo.resolution
            if not self.shadowMapInfo.renderer:
                print('create shadow-map renderer')
                try:
                    rt = dk.rendertarget.depthTarget(texSize, texSize, dk.rendertarget.DEPTH_24)
                except RuntimeError:
                    rt = dk.rendertarget.depthTarget(texSize, texSize, dk.rendertarget.DEPTH_16)
                rd = dk.Renderer(rt)
                self.shadowMapInfo.renderer = rd
                self.shadowMapInfo.renderer.polygonOffset = 1, 1
        else:
            self.shadowMapInfo.renderer = None

        text = ' Elapsed: {0:.6f} ({1:.2f} FPS) {2[0]:.0f}x{2[1]:.0f} Shadow: {3:.6f}'.format(delta,
                                                                                              1.0 / delta,
                                                                                              self.contentResolution,
                                                                                              shadowCalcElapsed)
        self.infoLabel.text = text
        self.infoLabel.redraw()
        self.bringChildToFront(self.infoLabel)
        self.redraw()

    def onRender(self, renderer):
        # super().onRender(renderer)
        # renderer.renderScene(self.scene, self.camera, SCENE_PASS_DEFAULT)

        materialInfo = {}

        scenePass = SCENE_PASS_DEFAULT

        if self.shadowMapInfo.renderer:
            camera = self.shadowMapInfo.camera
            rd = self.shadowMapInfo.renderer
            rd.clearDepthBuffer()
            rd.renderScene(self.scene, camera, SCENE_PASS_SHADOW_MAP)

            tex = self.shadowMapInfo.renderer.renderTarget.depthTexture()
            if not self.inspectMode and tex:
                bias = self.shadowMapInfo.biasMatrix
                tm = camera.viewMatrix() * camera.projectionMatrix() * bias
                materialInfo['shadowMapMatrix'] = tm.tuple
                materialInfo['shadowMap'] = (tex,)
                materialInfo['shadowMapSize'] = (self.shadowMapInfo.resolution,)
                texOffset = 1.0 / self.shadowMapInfo.resolution
                materialInfo['pcfOffset'] = (-texOffset, texOffset, 0.0, texOffset, texOffset, texOffset,
                                             -texOffset, 0.0, 0.0, 0.0, texOffset, 0.0,
                                             -texOffset, -texOffset, 0.0, -texOffset, texOffset, -texOffset)
                if self.shadowMapInfo.multisample:
                    scenePass = SCENE_PASS_SHADOWED_MULTISAMPLE
                else:
                    scenePass = SCENE_PASS_SHADOWED

        materialInfo['fogDensity'] = (0.00012,)
        materialInfo['fogColor'] = self.backgroundColor.tuple

        renderer.polygonOffset = 1, 1
        super().onRender(renderer)
        if self.inspectMode:
            renderer.renderScene(self.scene, self.inspectCamera, SCENE_PASS_DEFAULT, materialInfo=materialInfo)
            tm1 = self.camera.viewProjectionMatrix()
            tm2 = self.shadowMapInfo.camera.viewProjectionMatrix()
            tm1.inverse()
            tm2.inverse()

            self.cameraFrustumFaceMesh.setMaterialProperty('boxTransform', floatings=tm1.tuple)
            self.cameraFrustumLineMesh.setMaterialProperty('boxTransform', floatings=tm1.tuple)
            self.shadowFrustumFaceMesh.setMaterialProperty('boxTransform', floatings=tm2.tuple)
            self.shadowFrustumLineMesh.setMaterialProperty('boxTransform', floatings=tm2.tuple)
            renderer.renderScene(self.inspectScene, self.inspectCamera, 0)
        else:
            renderer.renderScene(self.scene, self.camera, scenePass, materialInfo=materialInfo)

    def onMouseDown(self, deviceId, buttonId, pos):
        super().onMouseDown(deviceId, buttonId, pos)
        if deviceId == 0:
            self.captureMouse(deviceId)

    def onMouseUp(self, deviceId, buttonId, pos):
        super().onMouseUp(deviceId, buttonId, pos)
        if deviceId == 0:
            self.releaseMouse(deviceId)

    def onMouseMove(self, deviceId, pos, delta):
        super().onMouseMove(deviceId, pos, delta)
        if deviceId == 0:
            if self.isMouseCapturedBySelf(deviceId):
                camera = self.camera if not self.inspectMode else self.inspectCamera

                dir = camera.direction()
                #up = self.camera.up()
                up = dk.Vector3(0,1,0)
                left = dir.cross(up)

                dX = delta.x * 0.01
                dY = delta.y * 0.01

                qY = dk.Quaternion(up, -dX)
                qX = dk.Quaternion(left, dY)
                rot = qX * qY

                pos = camera.position() - self.cameraInfo.target
                pos.transform(rot)
                pos += self.cameraInfo.target
                # minY = self.cameraInfo.target.y + self.cameraInfo.near + 2
                # if pos.y < minY: pos.y = minY
                if not self.inspectMode:
                    pos.y = max(pos.y, self.cameraInfo.target.y)
                camera.setView(pos, self.cameraInfo.target - pos, up)


    def onMouseWheel(self, deviceId, pos, delta):
        super().onMouseWheel(deviceId, pos, delta)
        if deviceId == 0:
            camera = self.camera if not self.inspectMode else self.inspectCamera
            pos = camera.position()
            dir = self.cameraInfo.target - pos

            d1 = dir.length()
            if self.inspectMode:
                s = delta.y * 1000
            else:
                s = delta.y * 100
            d2 = d1 + s
            d2 = max(d2, 25)
            if d1 != d2:
                up = camera.up()
                p = pos - self.cameraInfo.target
                p.normalize()
                p *= d2
                p += self.cameraInfo.target

                camera.setView(p, self.cameraInfo.target - p, up)

    def onShadowResolutionChanged(self, sender):
        self.shadowMapInfo.resolution = sender.value
        self.shadowMapInfo.renderer = None
        print(self.shadowMapInfo)

    def onShadowTypeChanged(self, sender):
        self.shadowMapInfo.type = sender.value
        print(self.shadowMapInfo)

    def onShadowBVectorValueChanged(self, sender):
        self.shadowMapInfo.bvector = sender.value
        print(self.shadowMapInfo)

    def onShadowMultiSampleValueChanged(self, sender):
        self.shadowMapInfo.multisample = sender.value
        print(self.shadowMapInfo)

    def onShadowInspdeModeValueChanged(self, sender):
        self.inspectMode = sender.value
        print('inspectMode: ', self.inspectMode)

    def onSliderValueChanged(self, slider):
        val = slider.value
        idx = slider.vectorIndex
        light = self.scene.lights[0]
        pos = list(light.position)
        pos[idx] = val
        light.position = tuple(pos)
        self.scene.updateLights()


if __name__ == '__main__':
    import app
    app.AppName = 'shadow test'
    app.MainFrame = Frame
    app.App((960, 640)).run()
