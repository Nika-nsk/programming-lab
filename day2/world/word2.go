package world

import(
	"strings"
	"os"
	"fmt"
)
type Point struct { X, Y int }

func LoadMaze(filename string) ([][]rune, Point, Point, error) {

data, err := os.ReadFile(filename)
if err != nil {
	fmt.Println(err)
return nil, Point{}, Point{}, err
}

	lines := strings.Split((strings.TrimSpace(string(data))), "\n")

	
	maze:=make([][]rune, len(lines))
	var start, end Point
	
	for i:=0; i<len(lines); i++ {
		 lineRunes := []rune(lines[i])
	
		 maze[i] = make([]rune, len(lineRunes))
		fmt.Printf(" %s\n", string(lineRunes))
        for j := 0; j < len(lineRunes); j++ {
            maze[i][j] = lineRunes[j]
		
			
			if maze[i][j] == 'S' {
				start = Point{j, i}
			} else if maze[i][j] == 'E' {
				end = Point{j, i}
			}
			
		}
	
	}
	
	
	return maze, start, end, nil


}