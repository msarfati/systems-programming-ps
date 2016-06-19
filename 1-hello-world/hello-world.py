#!/usr/bin/env python
import os

fout = os.open("foo", os.O_WRONLY | os.O_CREAT, 0o644)
os.write(fout, bytes("Hello world!", "utf-8"))
os.close(fout)
