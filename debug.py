import subprocess
import os

os.chdir("/workspace/problem_015")

test_input = """10
insert key1 100
insert key1 1
insert key1 2
insert key2 3
insert key2 4
insert key0 5
insert key0 6
insert key1 7
insert key2 8
find key1
"""

result = subprocess.run(["./code"], input=test_input, capture_output=True, text=True)
print("stdout:", repr(result.stdout))
print("stderr:", repr(result.stderr))
print("returncode:", result.returncode)
