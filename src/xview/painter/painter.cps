cdef	DEFINE_paintit()
	% paintit draws a line segment that follows the mouse when dragged
	/paintit { 0 setgray x1 y1 moveto x y lineto stroke } def
	
cdef	DEFINE_setxy()
	/setxy { % event => --   track where mouse was, for paintit
		/x1 x store
		/y1 y store
		begin
			/x XLocation store
			/y YLocation store
		end
	} def
	
cdef	painter_main()
	/PaintEventMgr [
		PointButton {
			10 dict begin
			currentcursorlocation
			/y exch store /x exch store
			/x0 x store /y0 y store
			/x1 x store /y1 y store
			paintit
			[	null {currentprocess killprocess }
				/UpTransition null eventmgrinterest
				MouseDragged { setxy paintit }
				null null eventmgrinterest
			] forkeventmgr pop
			end
		} /DownTransition currentcanvas eventmgrinterest
	] forkeventmgr def

cdef	painter_repaint()
	{
	erasepage
	}
