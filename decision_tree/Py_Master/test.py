import subprocess
process = subprocess.run([r"C:/2048_env/2048-tree/build.exe"], capture_output=True, text=True).stdout.strip()
move = process.stdout.strip()
print(move)