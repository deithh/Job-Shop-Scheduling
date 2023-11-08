import os

import sys
import asyncio

TRUNC = -1
Ks = [3,5,7]
TIMES = [1,2,5]



def extractJobsMachines(file):
    with open(file, "r") as f:
        temp = f.readline()
        temp = temp.split()
    tasks, machines = temp[0], temp[1]
    return tasks, machines




async def test(cmd, file, type, worktime, k):
    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)

    stdout, stderr = await proc.communicate()

    if proc.returncode != 0:
        exit()

    if stdout:
        time = stdout.decode().split("\n")[0]
        tasks, machines = extractJobsMachines(file)
        tasks = max(TRUNC, int(tasks))
        print(f"{time};{k};{type};{tasks};{machines};{worktime}")
        return stdout

        

async def _tester(cmd):

    proc = await asyncio.create_subprocess_shell(
        cmd,
        stdout=asyncio.subprocess.PIPE,
        stderr=asyncio.subprocess.PIPE)

    stdout, stderr = await proc.communicate()

    if proc.returncode != 0:
        exit()

    if stdout:
        if ("OK" not in stdout.decode()):
            raise





def list_txt_files(path):
    files = []
    for file in os.listdir(path):
        if file.endswith(".txt"):
            files.append(os.path.join(path, file))
    return files


def main(k, time):

    beasley = list_txt_files(os.path.join(os.getcwd(), "beasley/"))
    tailard = list_txt_files(os.path.join(os.getcwd(), "tailard/"))
    for file in beasley:
        schedulePipe = asyncio.run(test(f"./main {'O'} {file} {TRUNC} {k} {time}", file, "O", time, k))
        with open("temp.txt", "wb") as f:
            f.write(schedulePipe)
        asyncio.run(_tester(f"./chk_jsorl {file} {'temp.txt'}"))

    for file in tailard:
        asyncio.run(test(f"./main {'T'} {file} {TRUNC} {k} {time}",file, "T", time, k))


with open("csv.txt", "w") as sys.stdout:
    print(f"time;K;type;tasks;machines;worktime")
    for i in Ks:
        for j in TIMES:
            main(i, j)