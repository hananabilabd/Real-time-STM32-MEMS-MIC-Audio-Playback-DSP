
import pyaudio
import time
import numpy as np
from scipy import signal
import threading
import go
from PyQt4.uic import loadUiType
import sounddevice as sd

def getFFT(data, rate):
    """Given some data and rate, returns FFTfreq and FFT (half)."""
    data = data * np.hamming(len(data))
    fft = np.fft.fft(data)
    fft = np.abs(fft)
    # fft=10*np.log10(fft)
    freq = np.fft.fftfreq(len(fft), (1.0 / rate))
    return freq[:int(len(freq) / 2)], fft[:int(len(fft) / 2)]



class SWHear():
    """


        updatesPerSecond - how fast to record new data. Note that smaller
        numbers allow more data to be accessed and therefore high
        frequencies to be analyzed if using a FFT later
    """
    import go
    def __init__(self, device=None, rate=None, updatesPerSecond=10):
        #self.p = pyaudio.PyAudio()
        self.chunk = 4096  # gets replaced automatically
        self.updatesPerSecond = updatesPerSecond
        self.chunksRead = 0
        self.device = device
        self.rate = rate
        self.outdata=np.zeros((self.chunk,1))
        self.indata=np.zeros((self.chunk,1))
        nyq = self.rate / 2.0
        n1 = 100 / nyq
        n2 = 1000 / nyq
        n3 = 2000 / nyq
        n4 = 3000 / nyq
        n5 = 4000 / nyq

        self.b1, self.a1 = signal.butter(5, [n1, n2], btype='bandpass', analog=False, output='ba')
        self.b2, self.a2 = signal.butter(5, [n2, n3], btype='bandpass', analog=False, output='ba')
        self.b3, self.a3 = signal.butter(5, [n3, n4], btype='bandpass', analog=False, output='ba')
        self.b4, self.a4 = signal.butter(5, [n4, n5], btype='bandpass', analog=False, output='ba')
        self.b_stop, self.a_stop = signal.butter(5, [n1, n5], btype='bandstop', analog=False, output='ba')

        self.set_go_instance(self.go)
        #self.t = threading.Thread(target=self.stream_readchunk)


    def initiate(self):
        """run this after changing settings (like rate) before recording"""
        #self.chunk = int(self.rate / self.updatesPerSecond)
        self.datax = np.arange(self.chunk) / float(self.rate)
        #self.set_go_instance(go)



    ### STREAM HANDLING

    def stream_readchunk(self):
        """reads some audio and re-launches itself"""


        self.stream

        #self.i +=1
        #print self.i
        time.sleep(0.01)
        #sd.sleep(1000)

        self.stream_thread_new()

        #self.loop()
        #self.chunksRead += 1
        #sd.sleep(100)
        #time.sleep(0.05)
        #self.stream_readchunk()
        #self.stream.close

    def stream_thread_new(self):
        self.t = threading.Thread(target=self.stream_readchunk)
        #self.t = threading.Timer(10,self.stream_readchunk)
        #self.t2=threading.Thread(target=self.filtration)
        self.t.start()
        #self.t2.start()
        #time.sleep(0.01)


    def callback(self,indata, outdata, frames, time, status):
        if status:
            print(status)
        self.indata = indata
        self.data0 = self.indata.reshape(self.indata.shape[0], )
        # temp,c = self.stream.read(self.chunk)
        # self.data0 =temp.reshape(temp.shape[0])



        # self.data0 = np.fromstring(self.stream.read(self.chunk), dtype=np.int16)



        self.data1 = signal.lfilter(self.b1, self.a1, self.data0)
        self.data2 = signal.lfilter(self.b2, self.a2, self.data0)
        self.data3 = signal.lfilter(self.b3, self.a3, self.data0)
        self.data4 = signal.lfilter(self.b4, self.a4, self.data0)
        self.data_stop = signal.lfilter(self.b_stop, self.a_stop, self.data0)
        self.data = self.data1 * self.go.norm + self.data2 * self.go.norm2 + self.data3 * self.go.norm3 + self.data4 * self.go.norm4 + self.data_stop
        #self.data = self.data1 * 1 + self.data2 * 1 + self.data3 * 1 + self.data4 * 1 + self.data_stop
        # print len(self.data)
        # print(self.go.hanna)
        # self.fftx, self.fft = getFFT(self.data, self.rate)
        # print len(self.fftx) ,len(self.fft) ,self.rate
        # data = self.stream.read(self.chunk)
        # self.stream.write(self.data, self.chunk)
        # print (self.data.shape)
        a = self.data.reshape(self.data.shape[0], 1)
        self.outdata = np.array(a, dtype='float32')
        # o = a.astype('float32')
        # print o.shape
        # print o.shape[0]



        # self.stream2 = sd.OutputStream(channels=1, samplerate=self.rate)
        # self.stream2.start()
        # print self.o.shape
        # self.stream.write(self.o)

        # self.stream_readchunk()

        #outdata[:]= self.outdata

        outdata[:] = self.indata

        # print len(outdata)



    def stream_start(self):
        """adds data to self.data until termination signal"""
        self.initiate()
        print(" -- starting stream")
        self.keepRecording = True  # set this to False later to terminate stream
        self.data = None  # will fill up with threaded recording data
        self.fft = None
        self.dataFiltered = None  # same
        self.stream = sd.Stream(channels=1,samplerate=self.rate,blocksize=self.chunk,callback=self.callback)
        self.stream.start()

        #self.stream
        #self.stream_readchunk()
        self.stream_thread_new()

    def set_go_instance(self,go):
        self.go = go

    def print_go(self):
        print self.go

if __name__ == "__main__":
    import go
    ear = SWHear(updatesPerSecond=10,rate =44100)  # optinoally set sample rate here
    ear.stream_start()  # goes forever
    lastRead = ear.chunksRead
    #ear.set_go_instance(go)
    while True:
        ear.stream_readchunk()
        #time.sleep(.01)
        #while lastRead == ear.chunksRead:

        #print(ear.chunksRead, len(ear.data))
        #lastRead = ear.chunksRead
    print("DONE")
