package main

import "termbox"
import "time"

//---------------------------------------------------------------------------
// utility function for drawing strings
//---------------------------------------------------------------------------
func drawString(x int, y int, s string, fg uint16, bg uint16) {
	for _, r := range s {
		termbox.ChangeCell(x, y, r, fg, bg)
		x++
	}
}

//---------------------------------------------------------------------------
// chan based input
//---------------------------------------------------------------------------
func serveInput() chan *termbox.Event {
	event_sink := make(chan *termbox.Event)
	go func() {
		for {
			event := new(termbox.Event)
			result := event.Poll()
			if result > 0 {
				event_sink <- event
			}
		}
	}()
	return event_sink
}

//---------------------------------------------------------------------------
// drawing "rainbow" box around the terminal screen
//---------------------------------------------------------------------------
func drawBox(seed uint16) {
	w := termbox.Width()
	h := termbox.Height()

	color := uint16(seed)
	var x, x2, y, y2 int
	x2 = w - 1
	y2 = h - 1

	// horizontal
	for x = 0; x < w; x++ {
		termbox.ChangeCell(x, y, ' ', termbox.BLACK, color)
		termbox.ChangeCell(x, y2, ' ', termbox.BLACK, color)

		color++
		if color > 7 {
			color = 0
		}
	}
	x = 0

	// overdrawing corners here, but it's ok
	// vertical
	for y = 0; y < h; y++ {
		termbox.ChangeCell(x, y, ' ', termbox.BLACK, color)
		termbox.ChangeCell(x2, y, ' ', termbox.BLACK, color)

		color++
		if color > 7 {
			color = 0
		}
	}
}

func main() {
	err := termbox.Init()
	if err != nil {
		panic(err.String())
	}
	defer termbox.Shutdown()

	termbox.Clear()
	drawBox(0)
	termbox.Present()

	ticker := time.NewTicker(500000000)
	event_sink := serveInput()

	color := 0
	last_letter := " "

	mainloop: for {
		select {
		case <-ticker.C:
			color++;
			if color > 7 {
				color = 0
			}
		case event := <-event_sink:
			if event.Type == termbox.EVENT_KEY {
				if event.Key == termbox.KEY_ESC {
					break mainloop
				}
				if event.Ch != 0 {
					last_letter = string(event.Ch)
				} else {
					last_letter = " "
				}
			}
		}

		termbox.Clear()
		drawString(1,  1, last_letter, termbox.WHITE, termbox.BLACK)
		drawBox(uint16(color))
		termbox.Present()
	}
}
