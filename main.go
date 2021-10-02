package main

import (
	"fmt"
	"github.com/faiface/beep"
	"github.com/faiface/beep/speaker"
	"github.com/faiface/beep/wav"
	"log"
	"os"
	"time"
)

func main() {
	//f, _ := os.Create("./list1.txt")
	//
	//defer f.Close()
	//
	//for i := 0; i < 256; i++ {
	//	n := int64(i)
	//	pattern := fmt.Sprintf("%08b\n", n)
	//	num := strconv.Itoa(i)
	//	f.WriteString(num + "    " + pattern)
	//}
	//fmt.Println("file written")

	snrStreamer, format := createStream("snr")
	kickStreamer, _ := createStream("kick")
	hhStreamer, format := createStream("ch")

	speaker.Init(format.SampleRate, format.SampleRate.N(time.Second/30))

	snrBuffer := createBuffer(format, snrStreamer)
	kickBuffer := createBuffer(format, kickStreamer)
	hhBuffer := createBuffer(format, hhStreamer)

	hhHit := hhBuffer.Streamer(0, hhBuffer.Len())
	snrHit := snrBuffer.Streamer(0, snrBuffer.Len())
	kickHit := kickBuffer.Streamer(0, kickBuffer.Len())
	kickList := createPattern(79)
	snareList := createPattern(185)
	hhList := createPattern(233)

	count := 1
	for {
		counter := count % 8

		step(kickList, snareList, hhList, kickHit, snrHit, hhHit, counter)
		count++
	}
}

func step(kList, sList, hhList string, kLit, sLit, hLit beep.StreamSeeker, i int) {
	kItem := string(kList[i])
	sItem := string(sList[i])
	hItem := string(hhList[i])
	if kItem == "1" &&
		sItem == "1" &&
		hItem == "1" {
		fmt.Println("k s h")
		kLit.Seek(0)
		sLit.Seek(0)
		hLit.Seek(0)
		speaker.Play(kLit, sLit, hLit)
	}

	if kItem == "1" &&
		sItem == "1" &&
		hItem == "0" {
		fmt.Println("k s")
		kLit.Seek(0)
		sLit.Seek(0)
		speaker.Play(kLit, sLit)
	}

	if kItem == "1" &&
		sItem == "0" &&
		hItem == "1" {
		fmt.Println("k h")
		kLit.Seek(0)
		hLit.Seek(0)
		speaker.Play(kLit, hLit)
	}

	if kItem == "1" &&
		sItem == "0" &&
		hItem == "0" {
		fmt.Println("k")
		kLit.Seek(0)
		speaker.Play(kLit)
	}

	if kItem == "0" &&
		sItem == "1" &&
		hItem == "1" {
		fmt.Println("s h")
		sLit.Seek(0)
		hLit.Seek(0)
		speaker.Play(sLit, hLit)
	}

	if kItem == "0" &&
		sItem == "1" &&
		hItem == "0" {
		fmt.Println("s")
		sLit.Seek(0)
		speaker.Play(sLit)
	}

	if kItem == "0" &&
		sItem == "0" &&
		hItem == "1" {
		fmt.Println("h")

		hLit.Seek(0)
		speaker.Play(hLit)
	}
	if kItem == "0" &&
		sItem == "0" &&
		hItem == "0" {
		fmt.Println("nothing")
	}

	time.Sleep(250 * time.Millisecond)
}

func createBuffer(format beep.Format, streamer beep.StreamSeekCloser) *beep.Buffer {
	buffer := beep.NewBuffer(format)
	buffer.Append(streamer)
	streamer.Close()
	return buffer
}

func createStream(id string) (beep.StreamSeekCloser, beep.Format) {
	f, err := os.Open("./samples/" + id + ".wav")
	if err != nil {
		log.Fatal(err)
	}

	streamer, format, err := wav.Decode(f)
	if err != nil {
		log.Fatal(err)
	}
	return streamer, format
}

func createPattern(number int) string {
	n := int64(number)
	pattern := fmt.Sprintf("%08b\n", n)
	return pattern
}
