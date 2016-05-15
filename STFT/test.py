import pyublas
import numpy as np
import math
import STFT
import matplotlib.pyplot as plt

def showSTFT(x):
    print(x)
#    plt.plot(range(len(x)), x)
#    plt.show()
    print (x.shape)
    print (type(x))
    y, p = STFT.stft(x, 1024, 128)

    print(y)
    plt.imshow(y, interpolation='nearest', aspect='auto')
    plt.show()

    print(p)
    plt.imshow(p, interpolation='nearest', aspect='auto')
    plt.show()

""" synth data """
t = np.asarray(range(16000 * 10)) / 16000.
x = np.sin(1650. * t * 2. * np.pi) + np.sin(2200. * t * 2. * np.pi) / 2. + np.sin(4400. * t * 2. * np.pi) / 5.
showSTFT(x)


""" Real Audio File """

import wave
import scipy as sp
wf = wave.open("../sampleSounds/doremi.wav","rb")
print(wf.getnchannels())
print(wf.getnframes())
print(wf.getframerate())

def readWaveAsFloat(wf):
    length = wf.getnframes()
    data = wf.readframes(length)
    data = sp.fromstring(data, sp.int16)
#    data = [1 if e is None else 0 for e in data]
#    print(sum(data))
#    data = np.append(data, [0] * 1024)
    data = np.asarray(data, dtype="float64")
    data = data / 32768.
    print(data.shape)
    return data

data = readWaveAsFloat(wf)
wf.close()

showSTFT(data)
