import dk
import math


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
        self.animSpeedItems = None
        self.animMenuItems = None
        self.animMenuItems = None
        self.charMenuItems = None
        self.animControl = None
        self.animations = None
        self.sword = None
        self.character = None
        self.resourcePool = None
        self.menuBar = None
        self.scene = None
        self.mouseDown = set()

    def onLoaded(self):
        super().onLoaded()
        self.backgroundColor = dk.Color(0.22, 0.32, 1.0)

        charMenu = dk.ui.Menu()
        c1 = charMenu.addItem('Character 1')
        c1.filename = 'dil.DKMODEL'
        c1.callback = self.loadCharacter
        c2 = charMenu.addItem('Character 2')
        c2.filename = 'dih.DKMODEL'
        c2.callback = self.loadCharacter
        c3 = charMenu.addItem('Character 3')
        c3.filename = 'kon.DKMODEL'
        c3.callback = self.loadCharacter
        self.charMenuItems = [c1, c2, c3]

        animMenu = dk.ui.Menu()
        a1 = animMenu.addItem('Walk')
        a1.animIndex = 0
        a1.callback = self.changeAnimation
        a2 = animMenu.addItem('Run')
        a2.animIndex = 1
        a2.callback = self.changeAnimation
        a3 = animMenu.addItem('Attack')
        a3.animIndex = 2
        a3.callback = self.changeAnimation
        self.animMenuItems = [a1, a2, a3]

        speedMenu = dk.ui.Menu()
        s1 = speedMenu.addItem('0.5')
        s1.animSpeed = 0.5
        s1.callback = self.changeAnimationSpeed
        s2 = speedMenu.addItem('1.0')
        s2.animSpeed = 1.0
        s2.callback = self.changeAnimationSpeed
        s3 = speedMenu.addItem('2.0')
        s3.animSpeed = 2.0
        s3.callback = self.changeAnimationSpeed
        speedMenu.addSeparator()
        speedMenu.addItem('Reverse').enabled = False
        s4 = speedMenu.addItem('-0.5')
        s4.animSpeed = -0.5
        s4.callback = self.changeAnimationSpeed
        s5 = speedMenu.addItem('-1.0')
        s5.animSpeed = -1.0
        s5.callback = self.changeAnimationSpeed
        s6 = speedMenu.addItem('-2.0')
        s6.animSpeed = -2.0
        s6.callback = self.changeAnimationSpeed
        self.animSpeedItems = [s1, s2, s3, s4, s5, s6]

        ctrlMenu = dk.ui.Menu()
        animPlayStopMenuItem = ctrlMenu.addItem('Play')
        animPlayStopMenuItem.playing = False
        animPlayStopMenuItem.callback = self.togglePlayStop
        ctrlMenu.addSeparator()
        ctrlMenu.addItem('Speed').subMenu = speedMenu

        self.menuBar = dk.ui.Menu()
        self.menuBar.addItem('Models').subMenu = charMenu
        self.menuBar.addSeparator()
        self.menuBar.addItem('Animations').subMenu = animMenu
        self.menuBar.addSeparator()
        self.menuBar.addItem('Control').subMenu = ctrlMenu
        self.menuBar.addSeparator()

        self.addChild(self.menuBar)

        self.infoLabel = dk.ui.Label(' left-click: camera, right-click: light (two fingers move on mobile)')
        self.infoLabel.fontAttributes = dk.ui.font.attributes(14, outline=2)
        self.infoLabel.align = dk.ui.label.ALIGN_LEFT
        self.infoLabel.setBlendState(dk.blendstate.defaultAlpha)
        self.infoLabel.backgroundColor = dk.color.clear
        self.infoLabel.textColor = dk.color.white
        self.infoLabel.outlineColor = dk.color.black
        self.infoLabel.enabled = False
        self.addChild(self.infoLabel)

        self.resourcePool = dk.ResourcePool()
        self.resourcePool.addSearchPath(dk.appInstance().resourceDir + "/knights")

        # create scene
        self.scene = dk.Scene()
        self.character = None
        self.sword = self.resourcePool.loadResource('katana.DKMODEL')
        self.animations = [
            self.resourcePool.loadResource('walk.DKANIMATION'),
            self.resourcePool.loadResource('run.DKANIMATION'),
            self.resourcePool.loadResource('attack.DKANIMATION'),
        ]
        self.animControl = None

        self.scene.ambientColor = dk.Color(0.45, 0.45, 0.45)
        self.scene.lights.append(dk.light.directional(dk.Vector3(0, -1, 0), dk.Color(1, 1, 1)))
        self.scene.updateLights()

        # init camera
        self.camera = dk.Camera()
        self.cameraInfo = CameraInfo(fov=math.radians(80),
                                     near=10.0,
                                     far=1000.0,
                                     aspect=1.0,
                                     target=dk.Vector3(0, 80, 0))
        pos = dk.Vector3(0, 100, 120)
        up = dk.Vector3(0, 1, 0)
        self.camera.setView(pos, self.cameraInfo.target - pos, up)

        self.changeAnimation(self.animMenuItems[0])
        self.togglePlayStop(animPlayStopMenuItem)
        self.changeAnimationSpeed(self.animSpeedItems[1])
        self.screen().postOperation(self.loadCharacter, (self.charMenuItems[2],))


    def onUnload(self):
        super().onUnload()

        self.animSpeedItems = None
        self.animMenuItems = None
        self.animMenuItems = None
        self.charMenuItems = None

        self.animControl = None
        self.animations = None
        self.sword = None
        self.character = None
        self.resourcePool = None

        self.menuBar = None
        self.infoLabel = None
        self.scene = None

    def togglePlayStop(self, item, *args):
        if item and item.enabled:
            if self.animControl:
                item.playing = not item.playing
                if item.playing:
                    self.animControl.play()
                    item.text = 'Pause'
                else:
                    self.animControl.stop()
                    item.text = 'Play'


    def changeAnimationSpeed(self, item, *args):
        if item and item.enabled:
            if self.animControl:
                self.animControl.speed = item.animSpeed

                for i in self.animSpeedItems:
                    i.checked = i is item
                    i.enabled = i is not item

    def changeAnimation(self, item, *args):
        if item and item.enabled:
            index = item.animIndex
            oldControl = self.animControl
            self.animControl = self.animations[index].createLoopController()
            if oldControl:
                self.animControl.speed = oldControl.speed
                if oldControl.isPlaying():
                    self.animControl.play()
                else:
                    self.animControl.stop()
            else:
                self.animControl.speed = 1.0

            if self.character:
                self.character.setAnimation(self.animControl)

            for i in self.animMenuItems:
                i.checked = i is item
                i.enabled = i is not item

    def loadCharacter(self, item, *args):
        if item and item.enabled:
            self.sword.removeFromParent()
            if self.character:
                self.character.removeFromParent()

            file = item.filename
            for i in self.charMenuItems:
                i.checked = i is item
                i.enabled = i is not item

            self.character = self.resourcePool.loadResource(file)
            weapon = self.character.findDescendant('Weapon')
            if weapon:
                weapon.addChild(self.sword)

            self.scene.addObject(self.character)
            if self.animControl:
                self.character.setAnimation(self.animControl)

    def updatePerspective(self):
        w,h = self.contentResolution
        self.cameraInfo.aspect = w/h
        self.camera.setPerspective(self.cameraInfo.fov, self.cameraInfo.aspect, self.cameraInfo.near, self.cameraInfo.far)

    def layout(self):
        super().layout()
        bounds = self.bounds()
        if not self.menuBar.isLoaded():
            self.menuBar.load(self.screen())
        menuBarSize = self.menuBar.calculateFrameSize()
        self.menuBar.frame = dk.Rect(0,
                                     bounds.height - menuBarSize.height,
                                     # menuBarSize.width,
                                     bounds.width,
                                     menuBarSize.height)

        self.infoLabel.frame = dk.Rect(0, 0, bounds.width, 50)

        self.updatePerspective()

    def onUpdate(self, delta, tick, date):
        self.redraw()
        self.scene.update(delta, tick)

    def onRender(self, renderer):
        super().onRender(renderer)
        renderer.renderScene(self.scene, self.camera, 0)

    def onMouseDown(self, deviceId, buttonId, pos):
        super().onMouseDown(deviceId, buttonId, pos)
        self.captureMouse(deviceId)
        self.mouseDown.add((deviceId, buttonId))

    def onMouseUp(self, deviceId, buttonId, pos):
        super().onMouseUp(deviceId, buttonId, pos)
        if self.isMouseCapturedBySelf(deviceId):
            self.releaseMouse(deviceId)

        self.mouseDown.discard((deviceId, buttonId))

    def onMouseMove(self, deviceId, pos, delta):
        super().onMouseMove(deviceId, pos, delta)

        if self.isMouseCapturedBySelf(deviceId):

            moveCamera = deviceId == 0 and (0, 0) in self.mouseDown
            moveLight = deviceId == 1 or (0, 1) in self.mouseDown

            if moveCamera or moveLight:
                dir = self.camera.direction()
                # up = self.camera.up()
                up = dk.Vector3(0, 1, 0)
                left = dir.cross(up)

                dX = delta.x * 0.01
                dY = delta.y * 0.01

                qY = dk.Quaternion(up, -dX)
                qX = dk.Quaternion(left, dY)
                rot = qX * qY

                if moveCamera:      # camera move
                    pos = self.camera.position() - self.cameraInfo.target
                    pos.transform(rot)
                    pos += self.cameraInfo.target
                    self.camera.setView(pos, self.cameraInfo.target - pos, up)
                else:               # light move
                    light = self.scene.lights[0]
                    dir = light.direction()
                    rot.inverse()
                    dir.transform(rot)
                    light.setDirection(dir)
                    self.scene.updateLights()
