package config //берет номер порта и передает в .env

import "os"

type Config struct {
	ServerPort string
}


func LoadConfig() *Config {
	port := os.Getenv("SERVER_PORT")
	if port == "" {
		port = ":8080" 
	}
	return &Config{
		ServerPort: port,
	}
}
