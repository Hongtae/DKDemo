import os
import dk


class Window(dk.Window):
    minimumWidth = 400
    minimumHeight = 400

    def getContentMinSize(self):
        return self.minimumWidth, self.minimumHeight


class App(dk.App):

    def __init__(self, appName, mainFrameClass, desiredWindowSize=(1024, 768), *a, **k):
        super().__init__(*a, **k)
        self.mainFrameClass = mainFrameClass
        self.desiredWindowSize = max(desiredWindowSize[0], Window.minimumWidth),\
                                 max(desiredWindowSize[1], Window.minimumHeight)
        self.appName = appName

    def onInit(self):
        if self.isProxyInstance():
            # app 에서 호출됨.
            res1 = self.envPaths[dk.app.ENV_PATH_APP_RESOURCE]
            res2 = os.path.normpath(os.path.join(res1, 'Resources'))
            if os.path.exists(res2):
                self.resourceDir = res2
            else:
                self.resourceDir = res1
        else:
            # 파이썬에서 직접 사용됨.
            self.resourceDir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'Resources'))

        print('self.resourceDir:', self.resourceDir)

        # 사용 가능한 해상도 구함.
        displayBounds = self.displayBounds(0)
        contentBounds = self.screenContentBounds(0)
        print('displayBounds: ', displayBounds)
        print('contentBounds: ', contentBounds)

        platform = dk.platform()
        contentSize = dk.Size(self.desiredWindowSize)

        if 'ios' in platform or 'android' in platform:
            contentSize = dk.Size(contentBounds.size)
        else:
            if contentSize.width > contentBounds.width:
                contentSize.width = contentBounds.width
            if contentSize.height > contentBounds.height:
                contentSize.height = contentBounds.height

        print('main screen resolution: ', contentSize)

        window = Window(self.appName, contentSize)
        if window:
            dk.ui.view.DEFAULT_UI_SCALE = window.contentScaleFactor
            dk.ui.font.DEFAULT_FILENAME = 'NanumGothic.ttf'
            # dk.ui.font.DEFAULT_FILENAME = 'SeoulNamsanM.ttf'
            print('dk.ui.view.DEFAULT_UI_SCALE:', dk.ui.view.DEFAULT_UI_SCALE)
        screen = dk.Screen(window, self.mainFrameClass())
        if not screen:
            print('screen error!?')
            self.terminate(2)
        else:
            self.screen = screen
            self.screen.activeFrameLatency = 0
            self.screen.inactiveFrameLatency = 0
            self.screen.window.activate()

    def onExit(self):
        if hasattr(self, 'screen'):
            self.screen.terminate(True)
        self.screen = None


if __name__ == '__main__':
    class MainFrame(dk.ui.View):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, **kwargs)
            self.backgroundColor = dk.color.blue

        def onUnload(self):
            super().onUnload()
            raise SystemExit

    App('dummy', MainFrame).run()
