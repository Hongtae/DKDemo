import dk

class Frame(dk.ui.View):

    def onLoaded(self):
        super().onLoaded()

        resourcePool = dk.ResourcePool()
        resourcePool.addSearchPath(dk.appInstance().resourceDir)

        self.bgImage = resourcePool.loadResource('tree.jpg')
        self.bgTransform = dk.Matrix3()

    def onUnload(self):
        super().onUnload()

        self.bgImage = None

    def onRender(self, renderer):
        with renderer.contextForTexturedRects(self.bgImage) as r:
            r.add(self.bounds(), self.bgTransform)

