import subprocess
import argparse

DAY_KEY = "day"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("day", type=int)
    day = f"{vars(parser.parse_args())[DAY_KEY]:02}"
    subprocess.run(["mix", "format", "lib/*"])
    for test_type in ("samples", "puzzles"):
        for part in (1, 2):
            output = (
                subprocess.run(
                    [
                        "mix",
                        "run",
                        "-e",
                        f'IO.puts(Day{day}.run("input/{test_type}/{day}.txt", {part}))',
                    ],
                    capture_output=True,
                )
                .stdout.decode("utf-8")
                .strip()
            )
            print(output)
            with open(f"output/{test_type}/{day}-{part}.txt", "w") as file_handle:
                print(output, file=file_handle)


if __name__ == "__main__":
    main()
