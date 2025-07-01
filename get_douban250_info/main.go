package main

import (
	"encoding/csv"
	"fmt"
	"net/http"
	"os"
	"sync"
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
		link, _ := s.Find(".hd a").Attr("href") // Attr返回的第二个参数是是否存在这个属性，不存在则link为空

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
	defer writer.Flush() // 注意，通过writer.Writer写入的数据是存储在缓冲区的，执行Flush才能写入到文件

	writer.Write([]string{"标题", "评分", "链接"}) // Writer需要参数是字符串数组
	for _, m := range movies {
		writer.Write([]string{m.Name, m.Rating, m.Url})
	}
}

func main() {
	baseUrl := "https://movie.douban.com/top250"

	var wg sync.WaitGroup             // 计数器，在goroutine启动时+1，完成后-1
	taskChan := make(chan string, 10) // 运输通道，这里是运送string类型的通道
	resultChan := make(chan []Movie, 10)

	// 消费者，只管怎么处理，不管任务分发
	for i := 0; i < 3; i++ {
		wg.Add(1) // 表示即将开启一个新的goroutine

		// 这里的id只供打印参考，不会对并发有任何影响
		// 实际开启一个新的goroutine
		go func() {
			defer wg.Done() // wg.Done() 表示当前goroutine完成，计数器 - 1，defer将该操作推迟至函数末尾
			// 这里range等价于 <- taskChan，将taskChan头部元素传递给url
			for url := range taskChan {
				fmt.Println("抓取：", url)
				movies, err := fetchPage(url)
				if err != nil {
					fmt.Println(url, " 抓取失败：", err)
					continue
				}
				resultChan <- movies
			}
		}()
	}

	// 生产者，将url传给taskChan，然后消费者从taskChan队头取出元素开始消费
	for start := 0; start < 250; start += 25 {
		url := fmt.Sprintf("%s?start=%d", baseUrl, start)
		taskChan <- url
		time.Sleep(2 * time.Second)
	}

	// channel 被关闭后，里面“已有的任务”仍然可以被消费，直到 channel 彻底空了
	// 所以任务分发完毕后要立马关闭taskChan，不关闭的话 range taskChan会一直堵塞，永远无法退出
	close(taskChan)
	wg.Wait() // 等待所有 goroutine 执行完成
	close(resultChan)

	// 新开一个chan确保并发结果安全
	allMovies := []Movie{}
	for movies := range resultChan {
		allMovies = append(allMovies, movies...)
	}

	saveToCSV(allMovies)
	fmt.Println("结果已保存到 douban_top250.csv")
}
