# STFT module for python

## Requirements

You first need to install `boost` and `pyublas`.

Some  common Python libraries are also required
(such as `numpy`, `scipy`, `matplotlib`). `wave`, an audio I/O library, is also required.

## Example
The output is a 2-dim numpy array.
So you can visualize the spectrogram of an audio file usint `matplotlib` as follows.
![spectrogram example](./doc/spectrogram_example.png)
You can beutify this figure using some sophisticated functions of `matplotlib`

## python3

- Reinstall `boost`, specifying the version of python to be 3.
