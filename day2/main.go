package main


import (
	"myproject/world"
	"myproject/Entities"
	"os"
	"os/exec"
	"fmt"
)


func main(){ 
	var maze [][]rune
	var pos world.Point
	var err error
	var player entities.Point
	maze, pos, _, err = world.LoadMaze("mazes/mazes7.txt")
	
	if err != nil {
		fmt.Println("Ошибка загрузки:", err)
		return 
	}
	


// Отключаем канонический режим и эхо (отображение вводимых символов)
sttyConfig := exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1", "-echo")
sttyConfig.Run()
	
// Обязательно возвращаем настройки назад (sane), иначе терминал "сломается"
defer exec.Command("stty", "-F", "/dev/tty", "sane").Run()

var b = make([]byte, 3)
for {
	

	os.Stdin.Read(b)
	if (b[0] == 'q') {
	break
}
	char := rune(b[0])
    switch char {
    case 'E', 'e':
		if maze[player.X][player.Y] != '🧱' {
			player.Y-- 
		}
        player.Y--
		break 
    case 'd', 'D':
		if char != '🧱'{
        player.Y++ }
		break
    case 's', 'S':
        player.X-- 
		break
    case 'f', 'F':
        player.X++ 
		break
    default:
        fmt.Println("Нажата какая-то другая клавиша")

    
	}
	    // fmt.Printf("Вы нажали: %v\n", b)

		entities.Render(maze, player)
	if pos.Y < 0 { pos.Y = 0 }
		if pos.Y >= len(maze) { pos.Y = len(maze) - 1 }
}
	

	

}

