package main
import "fmt"
func main() {
var ( 
	  A float64
	  B float64
 	  C string
	)
fmt.Print("Введите первое число:")
_, errA := fmt.Scan(&A)
	if errA != nil {
		fmt.Println("Некорректное число. Пожалуйста, введите числовое значение.")
		return
	}
fmt.Print("Выберите операцию(+,-,/,*):")
fmt.Scan(&C)
fmt.Print("Введите второе число:")
_, errB := fmt.Scan(&B)
	if errB != nil {
		fmt.Println("Некорректное число. Пожалуйста, введите числовое значение.")
		return
	}
if C == "+" {
		fmt.Println("Результат:",A, "+", B,"=", A + B)
	} else if C == "-" {
		fmt.Println("Результат:",A, "-", B,"=", A - B)
	} else if C == "*" {
		fmt.Println("Результат:",A, "*", B,"=", A * B)
	} else if C == "/" {
		if B != 0 {
			fmt.Println("Результат:",A, "/", B, "=",A / B)
		} else {
			fmt.Println("Ошибка: деление на ноль!")
		}
	} else {
		fmt.Println("Неверная операция")
	}

}