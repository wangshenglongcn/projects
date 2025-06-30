## 爬取豆瓣250电影信息

### python 版本

1. 准备工作

该项目本地安装`python3.10.8`，安装后环境变量默认可以识别到`python`跟`pip`，视个人情况而定

安装 `requests` 和 `BeautifulSoup` 库
```shell
pip install requests beautifulsoup4
```

2. 执行脚本

用python3执行脚本
```shell
python main.py
```

3. 查看产物

产物为 `douban250.json`

### Go 版本

1. 准备工作

```shell
go mod init package_name  # 初始化
go get github.com/PuerkitoBio/goquery  # 安装goquery库，用来处理html文本，会下载到$GOPATH中
```
生成的文件：
`go.mod` -- 项目依赖清单，记录 `go` 版本以及依赖版本
`go.sum` -- 依赖校验和，避免被篡改

2. 执行脚本

```shell
go run main.go
```

3. 查看产物

产物目前为 `douban_top250.csv`
