package entities

import "fmt"

type Point struct {
	X, Y int
}

func Render(maze [][]rune, player Point) {
	fmt.Print("\033[H\033[2J") // Очистка экрана

	for y, row := range maze {
		for x, char := range row {
			
			// ВСТАВЛЯЕМ ТВОЙ КУСОК КОДА ЗДЕСЬ:
			if x == player.X && y == player.Y {
				fmt.Print("🏃") 
				continue
			}

			// Отрисовка остального
			if char == '#' {
				fmt.Print("🧱")
			} else if char == 'E' {
				fmt.Print("🏁")
			} else {
				fmt.Print("  ") // Пустое место
			}
		}
		fmt.Println()
	}
}