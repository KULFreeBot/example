#!/usr/bin/env python
from sys import argv
from pathlib import Path
from re import search
from csv import DictWriter


def parse(srcfile: Path, destfile: Path):
    with destfile.open("w") as csvfile:
        csv_writer = DictWriter(csvfile, ["t", "v", "fl", "fr", "bl", "br"])
        csv_writer.writeheader()
        for line in srcfile.open("r"):
            time = search(
                "\[([0-9][0-9]):([0-9][0-9]):([0-9][0-9]\.[0-9][0-9][0-9]),[0-9][0-9][0-9]+\]", line)
            data = search(
                "\[([0-9]+), ([0-9]+), ([0-9]+), ([0-9]+), ([0-9]+)\]", line)
            if time is not None and data is not None:
                t = 3600 * int(time.group(1)) + 60 * \
                    int(time.group(2)) + float(time.group(3))
                (v, fl, fr, bl, br) = (int(data.group(1)), int(data.group(2)),
                                       int(data.group(3)), int(data.group(4)), int(data.group(5)))
                csv_writer.writerow(
                    {"t": t, "v": v, "fl": fl, "fr": fr, "bl": bl, "br": br})
            else:
                raise RuntimeError(f"Failed to parse line: \n\t{line}")


if __name__ == "__main__":
    assert len(argv) == 2
    logfile = Path(argv[1])
    parse(logfile, logfile.with_suffix(".csv"))