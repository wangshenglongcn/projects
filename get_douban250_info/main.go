package main

import (
	"encoding/csv"
	"fmt"
	"net/http"
	"os"
	"time"

	"github.com/PuerkitoBio/goquery"
)

type Movie struct {
	Name   string
	Rating string
	Url    string
}

func fetchPage(url string) ([]Movie, error) {
	movies := []Movie{}

	// 构造请求，http.Get方法没有添加header等功能，所以这里不用
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return movies, err
	}
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)")

	client := http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		return movies, err
	}

	// defer延迟处理，不会立即执行，会等到主函数结束后从栈中弹出并执行
	defer resp.Body.Close()

	doc, err := goquery.NewDocumentFromReader(resp.Body)
	if err != nil {
		return movies, err
	}

	doc.Find(".grid_view .item").Each(func(i int, s *goquery.Selection) {
		title := s.Find(".title").First().Text()
		rating := s.Find(".rating_num").Text()
		link, _ := s.Find(".hd a").Attr("href")

		movies = append(movies, Movie{
			Name:   title,
			Rating: rating,
			Url:    link,
		})
	})
	return movies, nil
}

func saveToCSV(movies []Movie) {
	file, err := os.Create("douban_top250.csv")
	if err != nil {
		fmt.Println("创建文件失败，错误：", err)
		return
	}

	// 延迟关闭
	defer file.Close()

	writer := csv.NewWriter(file)
	defer writer.Flush()

	writer.Write([]string{"标题", "评分", "链接"})
	for _, m := range movies {
		writer.Write([]string{m.Name, m.Rating, m.Url})
	}
}

func main() {
	alls := []Movie{}
	baseUrl := "https://movie.douban.com/top250"
	for start := 0; start < 250; start += 25 {
		url := fmt.Sprintf("%s?start=%d", baseUrl, start)
		//
		movies, err := fetchPage(url)
		if err != nil {
			fmt.Println("抓取失败：", err)
			continue
		}
		alls = append(alls, movies...)
		time.Sleep(2 * time.Second)
	}

	saveToCSV(alls)
	fmt.Println("结果已保存到 douban_top250.csv")
}
