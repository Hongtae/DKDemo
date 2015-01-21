import dk


class Frame(dk.ui.View):
    def onLoaded(self):
        super().onLoaded()

        resourcePool = dk.ResourcePool()
        resourcePool.addSearchPath(dk.appInstance().resourceDir)
        image = resourcePool.loadResource('koo.jpg')

        subMenu1 = dk.ui.Menu()
        subMenu1.addItem('메뉴 아이템1')
        subMenu1.addItem('메뉴 아이템2')
        subMenu1.addItem('메뉴 아이템3')
        subMenu1.addSeparator()
        subMenu2 = dk.ui.Menu()
        subMenu2.addItem('서브 메뉴 아이템1')
        subMenu2.addItem('서브 메뉴 아이템2')
        subMenu1.addItem('서브 메뉴').subMenu = subMenu2
        subMenu3 = dk.ui.Menu()
        subMenu3.addItem('메뉴 아이템4').checked = True
        subMenu3.addItem('메뉴 아이템5').checked = True
        subMenu3.addItem('메뉴 아이템6')
        subMenu4 = dk.ui.Menu()
        subMenu4.addItem('메뉴 아이템7')
        subMenu4.addItem('메뉴 아이템8')

        self.menuBar = dk.ui.Menu()
        self.menuBar.addItem('메뉴 1').subMenu = subMenu1
        self.menuBar.addItem('메뉴 2').subMenu = subMenu3
        self.menuBar.addSeparator()
        self.menuBar.addItem('메뉴 3').subMenu = subMenu4
        self.addChild(self.menuBar)

        self.button1 = dk.ui.Button('버튼')
        self.button2 = dk.ui.Button('이미지 버튼', image=image)

        self.addChild(self.button1)
        self.addChild(self.button2)

        self.checkbox1 = dk.ui.Checkbox('체크 박스1 - 영역 기반', value=True)
        self.checkbox1.backgroundColor = dk.Color(0.8, 0.8, 0.8)
        self.checkbox1.interactOnlyInsideVisibleContentRect = False
        self.checkbox2 = dk.ui.Checkbox('체크 박스2 - 텍스트 기반', value=True)
        self.checkbox2.interactOnlyInsideVisibleContentRect = True
        self.addChild(self.checkbox1)
        self.addChild(self.checkbox2)

        self.radio1 = dk.ui.RadioButton('라디오 버튼 1', group=1, frame=dk.Rect(10, 10, 140, 48))
        self.radio2 = dk.ui.RadioButton('라디오 버튼 2', group=1, frame=dk.Rect(10, 10, 140, 48))
        self.radio3 = dk.ui.RadioButton('라디오 버튼 3', group=1, frame=dk.Rect(10, 10, 140, 48))
        self.radio1.setSelected()
        self.addChild(self.radio1)
        self.addChild(self.radio2)
        self.addChild(self.radio3)


        self.treeView = dk.ui.TreeView(frame=dk.Rect(20,20,200,400))
        self.treeView.contentSize = dk.Size(300, 500)

        item1 = self.treeView.addItem('트리 아이템 1')
        item11 = self.treeView.addItem('서브 아이템 1', parent=item1)
        item12 = self.treeView.addItem('서브 아이템 2', parent=item1)
        item11.state = item11.STATE_DISABLED
        self.treeView.addItem('서브-서브 아이템 1', parent=item11)
        self.treeView.addItem('서브-서브 아이템 2', parent=item12)
        item2 = self.treeView.addItem('트리 아이템 2')
        item21 = self.treeView.addItem('서브 아이템 1', parent=item2)

        for i in range(3, 10):
            item = self.treeView.addItem('트리 아이템 {}'.format(i))
            if i < 4:
                for j in range(1, 4):
                    item2 = self.treeView.addItem('서브 아이템 {}'.format(j), parent=item)
                    for k in range(1, 3):
                        self.treeView.addItem('서브-서브 아이템 {}'.format(k), parent=item2)

        self.treeView.borderWidth = 1
        self.treeView.scrollBarBorderWidth = 1
        self.treeView.showVerticalScrollBar = True
        self.treeView.showHorizontalScrollBar = True
        self.treeView.showZoomButton = True
        self.treeView.leftScrollBar = True
        self.treeView.expandAllItems()
        self.addChild(self.treeView)

        self.textField = dk.ui.TextField('텍스트 입력 필드')
        self.addChild(self.textField)

        self.progressView = dk.ui.View()
        self.progressView.backgroundColor = dk.Color(0.9, 0.9, 0.9)
        self.addChild(self.progressView)

        progressAnim = 20.0
        prog1 = dk.ui.ProgressView(frame=dk.Rect(30, 10, 160, 20))
        prog1.progressAnimation = progressAnim
        prog1.setProgress(1)
        prog2 = dk.ui.ProgressView(frame=dk.Rect(10, 30, 20, 120))
        prog2.backgroundColor = dk.Color(0.4, 0.4, 0.4)
        prog2.progressColor = dk.Color(1.0, 1.0, 0.7)
        prog2.progressAnimation = progressAnim
        prog2.vertical = True
        prog2.setProgress(1)
        prog3 = dk.ui.ProgressView(circularType=True, frame=dk.Rect(70, 50, 100, 100))
        prog3.progressColor = dk.Color(0.7, 0.7, 1.0)
        prog3.progressAnimation = progressAnim
        prog3.borderWidth = 1
        prog3.innerCircleRadius = 40
        prog3.setProgress(1)
        prog4 = dk.ui.ProgressView(circularType=True, frame=dk.Rect(40, 40, 30, 30))
        prog4.progressColor = dk.Color(1.0, 0.7, 0.7)
        prog4.progressAnimation = progressAnim
        prog4.borderWidth = 1
        prog4.innerCircleRadius = 0
        prog4.setProgress(1)

        self.progressView.addChild(dk.ui.Label('프로그래스 뷰 샘플', frame=dk.Rect(2,170,196,28)))
        self.progressView.addChild(prog1)
        self.progressView.addChild(prog2)
        self.progressView.addChild(prog3)
        self.progressView.addChild(prog4)




        class MyScrollView(dk.ui.ScrollView, dk.ui.TitledView):
            caption = '팝업 윈도우 (이미지뷰 스크롤뷰)'
            captionHeight = 24
            fontAttributes = dk.ui.font.attributes(14)
            pass

        imageView = dk.ui.ImageView(image=image, frame=dk.Rect(0, 0, 300, 300))
        imageView.enabled = False
        sv = MyScrollView(contentSize=dk.Size(imageView.frame.size), frame=dk.Rect(500, 120, 250, 250))
        sv.addChild(imageView)
        sv.borderWidth = 1
        sv.showHorizontalScrollBar = True
        sv.showVerticalScrollBar = True
        sv.showZoomButton = True
        self.addChild(sv)

    def layout(self):
        super().layout()

        bounds = self.bounds()
        if not self.menuBar.isLoaded():
            self.menuBar.load(self.screen())
        menuBarSize = self.menuBar.calculateFrameSize()
        self.menuBar.frame = dk.Rect(0, bounds.height - menuBarSize.height, bounds.width, menuBarSize.height)

        y = self.menuBar.frame.y - 80
        self.button1.frame = dk.Rect(10, y, 100, 60)
        self.button2.frame = dk.Rect(140, y, 100, 60)

        self.checkbox1.frame = dk.Rect(260, y+30, 200, 30)
        self.checkbox2.frame = dk.Rect(260, y, 200, 30)

        y -= 10
        for btn in (self.radio1, self.radio2, self.radio3):
            y -= 28
            btn.frame = dk.Rect(20, y, 160, 28)

        y -= 240
        self.treeView.frame = dk.Rect(10, y, 200, 230)

        y = self.checkbox2.frame.y - 80
        self.textField.frame = dk.Rect(240, y, 200, 30)

        y -= 230
        self.progressView.frame = dk.Rect(240, y, 200, 200)

    def onUnload(self):
        super().onUnload()

    def onRender(self, renderer):
        super().onRender(renderer)

    def doNothing(self, *args, **kwargs):
        print('do nothing!')
