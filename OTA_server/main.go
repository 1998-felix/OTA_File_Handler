package main

import (
	"fmt"
	"log"
	"net/http"
	"path/filepath"
)

var staticDir = filepath.FromSlash("/home/oh/Documents/Projects/OTA_File_Handler/OTA_server/static")

func handleBin(w http.ResponseWriter, r *http.Request) {
	http.ServeFile(w, r, filepath.Join(staticDir, "/firmware.bin"))
}

func handlehtml(w http.ResponseWriter, r *http.Request) {
	http.ServeFile(w, r, filepath.Join(staticDir, "/index.html"))
}

func main() {

	http.HandleFunc("/bin", handleBin)
	http.HandleFunc("/", handlehtml)
	fmt.Println("Starting server at port 8080")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		log.Fatal(err)
	}

}
