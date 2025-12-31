Import("env")
import subprocess

def get_version():
    try:
        result = subprocess.run(
            ["./get_version.sh"],
            capture_output=True,
            text=True,
            check=True,
            cwd=env["PROJECT_DIR"]
        )
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return "N/A"

version = get_version()
env.Append(CPPDEFINES=[("VERSION", f'\\"{version}\\"')])
print(f"Building with VERSION={version}")