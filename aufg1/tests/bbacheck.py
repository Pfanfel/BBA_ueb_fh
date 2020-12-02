#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import subprocess
import json
import argparse
import os

try:
    import ctypes
    kernel32 = ctypes.windll.kernel32
    kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)
except AttributeError:
    pass

class ExpectationNotMetError(Exception):
    pass

TERMCOLOR_RED = "\033[31m"
TERMCOLOR_GREEN = "\033[32m"
TERMCOLOR_BOLD = "\033[1m"
TERMCOLOR_RESET = "\033[0m"

def poor_mans_pgm_parser(pgmdata):
    if (not pgmdata):
        raise ExpectationNotMetError('Keine Daten erhalten.')
    channels = None
    if (pgmdata.startswith('P2')):
        channels = 1
    elif (pgmdata.startswith('P3')):
        channels = 3
    else:
        raise ExpectationNotMetError('Kann nur Portable Graymap (P2) und Portable Pixmap (P3) im ASCII Format verarbeiten. Die ersten Bytes dieser Daten sind: "%s"'%(pgmdata[:min(10,len(pgmdata))]))
    lines = pgmdata.split('\n')
    lines = [l for l in lines if not l.startswith('#')]
    pgmdata = '\n'.join(lines)
    tokens = pgmdata.split()
    def interpret_tokens(tokens):
      typeinfo = tokens[0]
      try:
          width = int(tokens[1])
          height = int(tokens[2])
          intensity = int(tokens[3])
      except ValueError as verr:
          raise ExpectationNotMetError(
              'Erwartete Zahl in PXM Kopfdaten, bei der Umwandlung fand ich jedoch "%s".'%(' '.join(verr.args))
          )
      _datasize = width*height*channels
      try:
        pixels = [int(t) for t in tokens[4:4+_datasize]]
      except ValueError as verr:
          raise ExpectationNotMetError(
              'Erwartete Zahl in PXM Bilddaten, bei der Umwandlung fand ich jedoch "%s".'%(' '.join(verr.args))
          )
      if (len(tokens) > 4+_datasize):
          return pixels+interpret_tokens(tokens[4+_datasize:])
      else:
          return pixels
    return interpret_tokens(tokens)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='BBA Testwerkzeug')
    parser.add_argument('test', type=open, help="Testdefinition")
    parser.add_argument('--dut', type=str, help="Zu testendes Programm", default=os.path.join('.','bba')) # TODO: type=executable file
    parser.add_argument('--expand', action='store_true', help="Befehle vor dem Anzeigen mit konkreten Daten füllen.")
    parser.add_argument('--keepgoing', action='store_true', help="Bei Fehlern mit nächstem Test weitermachen.")
    parser.add_argument('--imgdir', type=str, help="Ordner zu Referenzbildern", required=True) # TODO: type=directory
    args = parser.parse_args()
    tests = json.load(args.test)
    args.imgdir = os.path.join(args.imgdir,'') #fügt os passendes slash hinzu, sollte dieses fehlen
    environment = {"DUT":args.dut, "IMGDIR":args.imgdir}
    allok = True
    if (not tests):
        raise ValueError('Keine Tests definiert.')
    for test in tests:
        try:
            print('Teste nun "{}"…'.format(test["title"]))
            del test["title"]
            command_template = test["command"]
            del test["command"]
            command = command_template.format(**{k:'"%s"'%(v) for k,v in environment.items()})
            print('Befehl ist "{}{}{}"'.format(TERMCOLOR_BOLD, command if args.expand else command_template, TERMCOLOR_RESET))
            p = subprocess.run(command, stdout=subprocess.PIPE, shell=True, check=True, encoding='utf-8')
            if ("ask" in test):
                answer = None
                while not answer:
                    answer = input(test["ask"]+" (y/n) ")
                if (answer not in ["y","j"]):
                    raise ExpectationNotMetError('Erwartete positive Antwort.')
                del test["ask"]
            if ("expect" in test):
                expect = test["expect"]
                if ("bytes" in expect):
                    if (p.stdout != expect["bytes"]):
                        raise ExpectationNotMetError('Erwartete "{}", bekam "{}".'.format(expect["bytes"], p.stdout))
                    del expect["bytes"]
                if ("image" in expect):
                    reference = poor_mans_pgm_parser(open(expect["image"].format(**environment)).read())
                    delivered = poor_mans_pgm_parser(p.stdout)
                    if (len(reference) != len(delivered)):
                        raise ExpectationNotMetError(
                            'Referenz {} hat andere Anzahl von Pixeln. Erwartete {}, bekam {}.'.format(
                                expect["image"],
                                len(reference),
                                len(delivered)
                            )
                        )
                    adiff = [abs(r-d) for r,d in zip(reference, delivered)]
                    if (any(adiff)):
                        if (len(adiff) > 25):
                            print("Auflösung zu groß: Unterschiede werden nicht auf der Konsole gezeigt.")
                            # TODO: bilder als dateien schreiben
                        else:
                            print(reference, delivered, adiff)
                        raise ExpectationNotMetError('Referenz {} hat andere Werte.'.format(expect["image"]))
                    del expect["image"]
                if ("pixels" in expect):
                    reference = expect["pixels"]
                    delivered = poor_mans_pgm_parser(p.stdout)
                    if (reference != delivered):
                        raise ExpectationNotMetError('Referenz {} hat andere Werte.'.format(reference))
                    del expect["pixels"]
                if (expect):
                    raise NotImplementedError('Nicht implementierter erwarteter Wert "{}"'.format(", ".join(expect.keys())))
                del test["expect"]
            if (test):
                raise NotImplementedError('Nicht implementierter Test-Fall "{}"'.format(", ".join(test.keys())))
            print("{}OK{}".format(TERMCOLOR_GREEN,TERMCOLOR_RESET))
        except ExpectationNotMetError as enme:
            allok = False
            print("{}FEHLER:{} {}".format(TERMCOLOR_RED,TERMCOLOR_RESET, enme))
            if (not args.keepgoing):
                break
        except UnicodeDecodeError as ude:
            print("{}Nicht Implementiert:{} Binärdaten werden nicht unterstützt. Bitte nur mit ASCII-Bildern arbeiten.".format(TERMCOLOR_RED,TERMCOLOR_RESET))
            raise
    if (allok):
        print("{}{}Alles klar gut und so und von dem Test IHM sein Ende.{}".format(TERMCOLOR_BOLD,TERMCOLOR_GREEN,TERMCOLOR_RESET))
    else:
        print("{}{}ONEIM! Es snid Fehlre aufgetretne.{}".format(TERMCOLOR_BOLD,TERMCOLOR_RED,TERMCOLOR_RESET))
