package main

import (
	"log"
	"net/http"
	"path/filepath"
)

// To fix this for any server
var staticDir = filepath.FromSlash("/home/oh/Documents/Projects/OTA_File_Handler/OTA_server/static")
var total_index_requests int = 0
var total_bin_requests int = 0

func handleBin(w http.ResponseWriter, r *http.Request) {
	total_bin_requests += 1
	log.Printf("Download request # %v made", total_bin_requests)
	http.ServeFile(w, r, filepath.Join(staticDir, "/firmware.bin"))
}

func handlehtml(w http.ResponseWriter, r *http.Request) {
	total_index_requests += 1
	log.Printf("Download request # %v made", total_index_requests)
	http.ServeFile(w, r, filepath.Join(staticDir, "/index.html"))
}

func main() {

	http.HandleFunc("/bin", handleBin)
	http.HandleFunc("/", handlehtml)
	log.Println("Starting the server at port 8080 ")
	if err := http.ListenAndServe(":8080", nil); err != nil {
		log.Fatal(err)
	}

}
