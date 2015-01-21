import dk
import math

Sprite = dk.sprite.Sprite
Animation = dk.sprite.Animation


class Frame(dk.ui.View):
    def onLoaded(self):
        super().onLoaded()

        resourcePool = dk.ResourcePool()
        resourcePool.addSearchPath(dk.appInstance().resourceDir)

        self.bgImage = resourcePool.loadResource('tree.jpg')
        self.bgTransform = dk.Matrix3()

        tpack = resourcePool.openResourceStream('animals.plist')
        texAtlas = dk.sprite.TexturePack.createFromPlist(tpack, resourcePool)

        bear = Sprite(size=(160, 160), texturePack=texAtlas, name='brownbear_normal_0.png')
        bear.setTextureIds(Sprite.STATE_HIGHLIGHTED, 'brownbear_normal_1.png')
        bear.center = (80, 80)
        bear.buttonCallback = self.touchBear

        giraffe = Sprite(size=(200, 200), texturePack=texAtlas, name='giraffe_normal_0.png')
        giraffe.setTextureIds(Sprite.STATE_HIGHLIGHTED, 'giraffe_normal_1.png')
        giraffe.center = (560, 200)
        giraffe.buttonCallback = self.touchGiraffe

        panda = Sprite(size=(140, 100), texturePack=texAtlas, name='panda_normal_0.png')
        panda.setTextureIds(Sprite.STATE_HIGHLIGHTED, 'panda_normal_1.png')
        panda.center = (80, 280)
        panda.buttonCallback = self.touchPanda

        fox = Sprite(size=(140, 140), texturePack=texAtlas, name='fox_normal_0.png')
        fox.setTextureIds(Sprite.STATE_HIGHLIGHTED, 'fox_normal_1.png')
        fox.center = (380, 300)
        fox.buttonCallback = self.touchFox

        bounds = self.bounds()
        self.sprite = Sprite(center=bounds.center, size=bounds.size)
        self.sprite.addChild(bear)
        self.sprite.addChild(giraffe)
        self.sprite.addChild(panda)
        self.sprite.addChild(fox)

    def onUnload(self):
        super().onUnload()

        self.bgImage = None
        self.sprite = None

        import gc
        gc.collect()

    def layout(self):
        super().layout()
        bounds = self.bounds()
        self.sprite.center = bounds.center
        self.sprite.size = bounds.size

    def onUpdate(self, delta, tick, date):
        super().onUpdate(delta, tick, date)
        self.sprite.update(delta, tick)
        self.redraw()

    def onRender(self, renderer):
        with renderer.contextForTexturedRects(self.bgImage) as r:
            r.add(self.bounds(), self.bgTransform)
        self.sprite.render(renderer)

    def onMouseDown(self, deviceId, buttonId, pos):
        self.sprite.mouseDown(deviceId, self.sprite.convertPoint(pos))
        return super().onMouseDown(deviceId, buttonId, pos)

    def onMouseUp(self, deviceId, buttonId, pos):
        self.sprite.mouseUp(deviceId, self.sprite.convertPoint(pos))
        return super().onMouseUp(deviceId, buttonId, pos)

    def onMouseMove(self, deviceId, pos, delta):
        self.sprite.mouseMove(deviceId, self.sprite.convertPoint(pos))
        return super().onMouseMove(deviceId, pos, delta)

    def touchBear(self, sp):
        size = self.sprite.size

        def fin(ani):
            sp.state = Sprite.STATE_NORMAL

        center = sp.center
        ani = Animation(center, 3.0, fin)
        if center[0] > size[0] * 0.5:
            ani.addFrame(3.0, (size[0] * 0.5 - 100, center[1]))
        else:
            ani.addFrame(3.0, (size[0] * 0.5 + 100, center[1]))

        sp.setAnimation('center', ani)
        sp.state = Sprite.STATE_DISABLED


    def touchGiraffe(self, sp):
        ani = Animation((1.0, 1.0), 3.0, interpolate=dk.sprite.INTERPOLATE_CATMULL_ROM)
        ani.addFrame(2.0, (3.0, 3.0))
        ani.addFrame(2.5, (3.0, 3.0))
        ani.addFrame(2.6, (1.5, 1.5))
        ani.addFrame(2.7, (2.0, 2.0))
        ani.addFrame(2.75, (0.8, 0.8))
        ani.addFrame(2.8, (1.3, 1.3))
        ani.addFrame(2.9, (0.9, 0.9))
        ani.addFrame(2.95, (1.2, 1.2))
        ani.addFrame(3.0, (1.0, 1.0))

        sp.setAnimation('scale', ani)

    def touchPanda(self, sp):
        ani1 = Animation((1.0, 1.0), 3.0, interpolate=dk.sprite.INTERPOLATE_CATMULL_ROM)
        ani1.addFrame(1.5, (2.0, 2.0))
        ani1.addFrame(3.0, (1.0, 1.0))
        ani2 = Animation((0.0,), 3.0, interpolate=dk.sprite.INTERPOLATE_HERMITE)
        ani2.addFrame(1.0, (math.pi * 6,))
        ani2.addFrame(3.0, (math.pi * 9,))
        sp.setAnimation('scale', ani1)
        sp.setAnimation('rotate', ani2)

    def touchFox(self, sp):
        ani = Animation((1.0,), 3.0, interpolate=dk.sprite.INTERPOLATE_HERMITE)
        ani.addFrame(1.0, (0.0,))
        ani.addFrame(2.0, (0.0,))
        ani.addFrame(3.0, (1.0,))
        sp.setAnimation('alpha', ani)
