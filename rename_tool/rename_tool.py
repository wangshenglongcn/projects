import os
import argparse

def rename_files(dir, pre, suf, dry):
    if not os.path.isdir(dir):
        print(f"目录不存在{dir}")
        return

    count = 1
    # os.walk返回文件夹路径、当前文件夹下的子目录list、当前文件夹下的文件list，这里只用到1、3
    for folder, _, files in os.walk(dir):
        for file in files:
            if file.endswith(suf):
                old_path = os.path.join(folder, file)
                new_name = f"{pre}_{count}{suf}"
                new_path = os.path.join(folder, new_name)
                # 如果文件已存在则跳过
                if os.path.exists(new_path):
                    count += 1
                    continue

                if (dry):
                    print(f"[预览] {old_path} 将要修改为：{new_path}")
                else:
                    # try-except捕获异常
                    try:
                        os.rename(old_path, new_path)
                        print(f"{old_path} 成功修改为：{new_path}.")
                        count += 1
                    except Exception as e:
                        print(f"重命名 {old_path} 失败，错误：{e}")


if __name__ == "__main__":  # 只有当前文件运行时才会执行以下操作
    # argparse定义传入参数信息，传参必需指定，比如python3 rename_tool.py --dir test
    parser = argparse.ArgumentParser(description="批量重命名指定目录的文件")
    parser.add_argument("--dir", required=True, help="目标文件夹路径")
    parser.add_argument("--pre", default="image", help="新文件名前缀")
    parser.add_argument("--suf", default=".jpg", help="文件后缀，如.jpg或.png")
    parser.add_argument("--dry", action="store_true", help="预览将要修改哪些文件")
    args = parser.parse_args()
    rename_files(args.dir, args.pre, args.suf, args.dry)
