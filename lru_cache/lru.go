package main

import (
	"fmt"
)

// 定义双链表
type Node struct {
	key, val   int   // 存储节点对应的key和value
	prev, next *Node // 指向前一个节点以及后一个节点的指针
}

var dummy = &Node{key: 0, val: 0} // dummy 是一个哨兵节点
var cache = map[int]*Node{}       // cache 是哈希表，存储key-*Node键值对，以便能正常添加删除
var capacity int                  // capacity 缓存最大容量
var size = 0                      // size 记录当前节点数量

func add(node *Node) {
	// 指向右侧的指针修改
	node.next = dummy.next
	dummy.next = node

	// 指向左侧的指针修改
	node.prev = dummy
	node.next.prev = node

	size++
}

func remove(node *Node) {
	// 改变指针指向，go是垃圾回收语言，不需要手动删除
	node.next.prev = node.prev
	node.prev.next = node.next

	size--
}

func initList() {
	// 初始化哨兵节点指针
	dummy.next = dummy
	dummy.prev = dummy
}

func printNode() {
	// 从头到尾打印当前链表内节点
	fmt.Print("操作完成，以下为当前数据：\n")
	cur := dummy.next
	for i := 0; i < size; i++ {
		fmt.Printf("%d %d\n", cur.key, cur.val)
		cur = cur.next
	}
	fmt.Print("=======================\n")
}

func main() {
	initList()

	fmt.Print("请输入LRU缓存的容量：")
	fmt.Scan(&capacity)
	for {
		fmt.Print("请输入你想进行的操作（add/delete/quit）：")
		var op string
		fmt.Scan(&op)

		if op == "quit" {
			break
		}

		switch op {
		case "add":
			{
				fmt.Print("请输入一对键值对：")
				var key, value int
				fmt.Scan(&key, &value)
				node := cache[key]
				if node == nil {
					node = &Node{key: key, val: value}
					add(node)
					cache[key] = node
				} else {
					node.val = value
					remove(node)
					add(node)
				}
				if size > capacity {
					last := dummy.prev
					remove(last)
					delete(cache, last.key)
				}
			}
		case "delete":
			{
				fmt.Print("请输入要删除的key：")
				var key int
				fmt.Scan(&key)
				node := cache[key]
				if node != nil {
					remove(node)
					delete(cache, key)
				} else {
					fmt.Printf("%d 不存在\n", key)
				}
			}
		}
		printNode()
	}
}
