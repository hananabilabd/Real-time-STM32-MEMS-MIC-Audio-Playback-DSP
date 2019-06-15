
import pyaudio
import time
import numpy as np
from scipy import signal
import threading
import go
from PyQt4.uic import loadUiType
import sounddevice as sd
#sd.default.latency = 'high'
def getFFT(data, rate):
    """Given some data and rate, returns FFTfreq and FFT (half)."""
    data = data * np.hamming(len(data))
    fft = np.fft.fft(data)
    fft = np.abs(fft)
    # fft=10*np.log10(fft)
    freq = np.fft.fftfreq(len(fft), (1.0 / rate))
    return freq[:int(len(freq) / 2)], fft[:int(len(fft) / 2)]



class SWHear():

    def __init__(self, device=None, rate=None, updatesPerSecond=10):
        #self.p = pyaudio.PyAudio()
        self.chunk = 1000  # gets replaced automatically
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

        #self.set_go_instance(self.go)



    def initiate(self):
        """run this after changing settings (like rate) before recording"""
        #self.chunk = int(self.rate / self.updatesPerSecond)
        self.datax = np.arange(self.chunk) / float(self.rate)
        #self.set_go_instance(go)



    ### STREAM HANDLING

    def stream_readchunk(self):
        """reads some audio and re-launches itself"""





        ##self.data0 = self.indata.reshape(self.indata.shape[0], )
        temp,c = self.stream.read(self.chunk)
        self.data0 =temp.reshape(temp.shape[0])






        self.data1 = signal.lfilter(self.b1, self.a1, self.data0)
        self.data2 = signal.lfilter(self.b2, self.a2, self.data0)
        self.data3 = signal.lfilter(self.b3, self.a3, self.data0)
        self.data4 = signal.lfilter(self.b4, self.a4, self.data0)
        self.data_stop = signal.lfilter(self.b_stop, self.a_stop, self.data0)
        self.data = self.data1 * self.go.norm + self.data2 * self.go.norm2 + self.data3 * self.go.norm3 + self.data4 * self.go.norm4 + self.data_stop
        ##self.data = self.data1 * 1 + self.data2 * 1 + self.data3 * 1 + self.data4 * 1 + self.data_stop

        self.fftx, self.fft = getFFT(self.data, self.rate)

        # #]print len(self.fftx) ,len(self.fft) ,self.rate



        a = self.data.reshape(self.data.shape[0], 1)
        self.o = np.array(a, dtype='float32')
        ## o = a.astype('float32')

        # #print o.shape[0]



        self.stream_2.write(self.o)




        self.stream_thread_new()



    def stream_thread_new(self):
        self.t = threading.Thread(target=self.stream_readchunk)
        self.t.start()








    def stream_start(self):
        """adds data to self.data until termination signal"""
        self.initiate()
        print(" -- starting stream")
        self.keepRecording = True  # set this to False later to terminate stream
        self.data = None  # will fill up with threaded recording data
        self.fft = None
        self.dataFiltered = None  # same
        self.stream = sd.Stream(channels=1,samplerate=self.rate,blocksize=self.chunk)
        self.stream_2 = sd.OutputStream(channels=1, samplerate=self.rate, blocksize=self.chunk)
        self.stream.start()
        self.stream_2.start()

        #self.stream_readchunk()
        self.stream_thread_new()

    def set_go_instance(self,go):
        self.go = go

    def print_go(self):
        print self.go

if __name__ == "__main__":
    print ("HIIII")
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
