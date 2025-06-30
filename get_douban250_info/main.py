import requests
import json
from bs4 import BeautifulSoup
from concurrent.futures import ThreadPoolExecutor

"""
1. 豆瓣一部分结构如下
div.info
    div.hd
        a带链接
            多个span，不同版本的标题
    div.bd
        p1 导演、主演 \n 时间、国家、标签
        p2 简短评价
        span1 分数
        span2 多少人评价

2. 随页数url变化规律
https://movie.douban.com/top250?start=25&filter=
https://movie.douban.com/top250?start=50&filter=

"""


def send_request(url, start):
    # 使用requests发送一次请求
    headers = {
        "user-agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36"
    }
    params = {"start": start, "filter": ""}
    try:
        resp = requests.get(url, headers=headers, params=params)
        resp.raise_for_status()
        return resp.text
    except Exception as e:
        print(f"请求失败，错误：{e}")
        return None


def handle(content):
    if not content:
        return []

    soup = BeautifulSoup(content, "html.parser")
    divs = soup.find_all("div", class_="info")

    movie_info = []
    for div in divs:
        movie_info.append(parse_movie_info(div))

    return movie_info


def parse_movie_info(div):
    # 处理集成一部电影的相关信息
    # 标题、链接部分
    hd = div.find("div", class_="hd")
    hd_a = hd.find("a")
    title = hd_a.get_text(strip=True)
    url = hd_a.get("href")

    # 评分、评价人数等
    bd = div.find("div", class_="bd")
    p = bd.find_all("p")
    span = bd.find_all("span")
    introduce = p[0].get_text(strip=True)
    note = p[1].get_text(strip=True) if len(p) > 1 else ""
    rating = span[1].get_text(strip=True)
    rating_cnt = span[3].get_text(strip=True)

    return {
        "title": title,
        "url": url,
        "introduce": introduce,
        "note": note,
        "rating": rating,
        "rating_cnt": rating_cnt,
    }


def write_to_json(data, filename):
    # 数据写入json文件
    try:
        with open(filename, mode="w", encoding="utf-8") as f:
            json.dump(data, f, ensure_ascii=False, indent=4)

        print(f"成功将数据写入json文件{filename}")
    except Exception as e:
        print(f"写入文件{filename}失败，错误：{e}")


def fetch_page_data(url, page):
    start = page * 25
    content = send_request(url, start)
    return handle(content)


if __name__ == "__main__":
    url = "https://movie.douban.com/top250"
    output = "douban250.json"

    movie_info = []
    with ThreadPoolExecutor(max_workers=3) as executor:
        futures = [executor.submit(fetch_page_data, url, i) for i in range(10)]
        for future in futures:
            movie_info += future.result()

    write_to_json(movie_info, output)
