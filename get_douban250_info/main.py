import requests

if __name__ == "__main__":
    url = "https://movie.douban.com/top250"
    resp = requests.get(url)
    print(res.json())
