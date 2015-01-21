
# DKDemo 메인 스크립트.
# 메인 메뉴 화면 UI를 구성함.

import dk
import gc
import inspect


import car
import shadow
import sprite
import ui
import anim
import kine

class MainFrame(dk.ui.View):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.backgroundColor = dk.Color(0.6, 0.6, 0.6)
        self.demoFrame = None

    def onLoaded(self):
        super().onLoaded()
        items = [
            (kine.Frame, '키네마틱 애니메이션'),
            (shadow.Frame, '그림자 샘플'),
            (sprite.Frame, '2D 스프라이트'),
            (car.Frame, '자동차 물리'),
            (ui.Frame, 'UI 샘플'),
            (anim.Frame, '캐릭터 애니메이션'),
        ]

        btnFontAttr = dk.ui.font.attributes(16, outline=2)

        self.buttons = []
        btnY = 10
        for itemClass, itemText in items:
            btn = dk.ui.Button(itemText, frame=dk.Rect(0, 0, 160, 60))
            btn.demoClass = itemClass
            btn.addTarget(self, self.openDemo)
            btn.fontAttributes = btnFontAttr
            self.addChild(btn)
            self.buttons.append(btn)

        self.demoFrame = None

        self.infoButton = dk.ui.Button('Info', frame=dk.Rect(0, 0, 80, 40))
        self.infoButton.fontAttributes = btnFontAttr
        self.infoButton.setTextColor(dk.Color(1, 1, 1, 1))
        self.infoButton.setOutlineColor(dk.Color(0.25, 0, 0, 1))
        self.infoButton.backgroundColor = dk.Color(1, 0, 0, 0.75)
        self.infoButton.backgroundColorHighlighted = dk.Color(1, 0.5, 0.5)
        self.infoButton.backgroundColorActivated = dk.Color(0.8, 0, 0)
        self.infoButton.addTarget(self, self.showInfoView)
        self.addChild(self.infoButton)

        self.closeButton = dk.ui.Button('Close', frame=dk.Rect(0, 0, 80, 40))
        self.closeButton.fontAttributes = btnFontAttr
        self.closeButton.setTextColor(dk.Color(1, 1, 1, 1))
        self.closeButton.setOutlineColor(dk.Color(0.25, 0, 0, 1))
        self.closeButton.backgroundColor = dk.Color(1, 0, 0, 0.75)
        self.closeButton.backgroundColorHighlighted = dk.Color(1, 0.5, 0.5)
        self.closeButton.backgroundColorActivated = dk.Color(0.8, 0, 0)
        self.closeButton.addTarget(self, self.closeDemo)
        self.addChild(self.closeButton)

        self.screen().postOperation(self.runGC, ())

    def onUnload(self):
        self.demoFrame = None
        self.infoButton = None
        self.closeButton = None
        self.buttons = []

        for c in self.children():
            c.unload()
            c.removeFromParent()

        super().onUnload()

        app = dk.App.instance()
        if app:
            app.terminate(990)
        else:
            raise SystemExit

    def layout(self):
        super().layout()

        if self.demoFrame:
            if isinstance(self.demoFrame, dk.ui.View):
                self.demoFrame.frame = self.contentBounds()
            else:
                t = dk.LinearTransform2()
                t.scale(self.contentScale)
                self.demoFrame.transform = dk.AffineTransform2(t).matrix3()

        btnSize = (100, 50)
        padding = 5
        width, height = self.contentScale
        x = width - btnSize[0] - padding
        self.closeButton.frame = dk.Rect(x, padding, *btnSize)
        x = x - btnSize[0] - padding
        self.infoButton.frame = dk.Rect(x, padding, *btnSize)

        columns = 3
        leftMargin, rightMargin, topMargin, bottomMargin = 40, 40, 80, 140
        padding = 10
        rect = self.bounds()
        rect.origin = dk.Point(rect.origin) + dk.Point(leftMargin, bottomMargin)
        rect.size = dk.Size(rect.size) - dk.Size(leftMargin + rightMargin, bottomMargin + topMargin)
        from math import ceil
        rows = ceil(len(self.buttons) / columns)

        btnSize = int((rect.width - padding * (columns-1)) / columns), int((rect.height - padding * (rows-1)) / rows)
        index = 0
        for btn in self.buttons:
            x = int(index % columns)
            y = int(index / columns)
            rc = dk.Rect()
            rc.origin = dk.Point(x * (btnSize[0]+padding), y * (btnSize[1]+padding)) + rect.origin
            rc.size = btnSize
            btn.frame = rc
            index += 1

    def showInfoView(self, *args):
        pool = dk.ResourcePool()
        pool.addSearchPath(dk.appInstance().resourceDir)

        class InfoView(dk.ui.TitledView):
            def onLoaded(self):
                super().onLoaded()
                labels = [
                    dk.ui.Label('DK Game Library'),
                    dk.ui.Label('DKDemo 프로젝트는 DKLib 데모용 샘플 입니다.'),
                    dk.ui.Label('DK 엔진은 C++ 로 되어있으며,'),
                    dk.ui.Label('파이썬 스크립트를 사용할 수 있습니다.'),
                    dk.ui.Label('이 데모 프로그램은 모두 스크립트로 되어 있으며,'),
                    dk.ui.Label('모든 스크립트파일은 Scripts 폴더에 있습니다.'),
                    dk.ui.Label('DK 엔진 구조는 C++ 헤더 파일을 참조 하세요.'),
                    dk.ui.Label('제작: 김홍태, Copyright (c) 2015.'),
                ]
                # font = dk.ui.font.attributes(12, file='SeoulNamsanM.ttf')
                font = dk.ui.font.attributes(12, file='NanumGothic.ttf')
                w, h, y = 300, 24, 260
                for la in labels:
                    y -= h
                    la.fontAttributes = font
                    la.frame = dk.Rect(0, y, w, h)
                    self.addChild(la)

        def dismiss(*a):
            dk.ui.Modal.modalForView(self).dismiss(99)

        infoView = InfoView(caption='About DKDemo', frame=dk.Rect(0, 0, 300, 300))
        btn = dk.ui.Button('확인', frame=dk.Rect(10, 10, 100, 40))
        btn.addTarget(self, dismiss)
        infoView.addChild(btn)

        modal = dk.ui.Modal(infoView)
        modal.present(self)


    def openDemo(self, button):
        demoClass = button.demoClass
        print('create sample frame: ', demoClass.__name__)
        self.demoFrame = demoClass(frame=self.bounds())
        if isinstance(self.demoFrame, dk.ui.View):
            self.demoFrame.frame = self.contentBounds()
        else:
            t = dk.LinearTransform2()
            t.scale(self.contentScale)
            self.demoFrame.transform = dk.AffineTransform2(t).matrix3()

        self.addChild(self.demoFrame)
        self.bringChildToFront(self.infoButton)
        self.bringChildToFront(self.closeButton)
        # self.closeButton.hidden = False

    def closeDemo(self, *args):
        if self.demoFrame:
            self.demoFrame.removeFromParent()
            self.demoFrame = None
            self.screen().postOperation(self.runGC, ())
        else:
            self.screen().window.close()

    def runGC(self):
        print('running GC')
        items = gc.collect()
        print('gc.collect() returns: ', items)


if __name__ == '__main__':
    import app
    app.App('DKDemo', MainFrame, (800, 450)).run()
