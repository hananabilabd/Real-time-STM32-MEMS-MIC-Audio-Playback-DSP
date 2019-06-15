"""
this is a stripped down version of the SWHear class.
It's designed to hold only a single audio sample in memory.
check my githib for a more complete version:
    http://github.com/swharden
"""

import pyaudio
import time
import numpy as np
from scipy import signal
import threading
import go
from PyQt4.uic import loadUiType
print ("HIOI")
def getFFT(data, rate):
    """Given some data and rate, returns FFTfreq and FFT (half)."""
    data = data * np.hamming(len(data))
    fft = np.fft.fft(data)
    fft = np.abs(fft)
    # fft=10*np.log10(fft)
    freq = np.fft.fftfreq(len(fft), (1.0 / rate))
    return freq[:int(len(freq) / 2)], fft[:int(len(fft) / 2)]


#Ui_MainWindow, QMainWindow = loadUiType('ui_main.ui')
class SWHear():
    """
    The SWHear class is provides access to continuously recorded
    (and mathematically processed) microphone data.

    Arguments:

        device - the number of the sound card input to use. Leave blank
        to automatically detect one.

        rate - sample rate to use. Defaults to something supported.

        updatesPerSecond - how fast to record new data. Note that smaller
        numbers allow more data to be accessed and therefore high
        frequencies to be analyzed if using a FFT later
    """
    import go
    def __init__(self, device=None, rate=None, updatesPerSecond=10):
        self.p = pyaudio.PyAudio()
        self.chunk = 4096  # gets replaced automatically
        self.updatesPerSecond = updatesPerSecond
        self.chunksRead = 0
        self.device = device
        self.rate = rate
        #self.form=go.ExampleApp()
        #self.form.valuechange()

    ### SYSTEM TESTS////////////////////////////////////////////////////////////////////////////////////////////////////////

    def valid_low_rate(self, device):
        """set the rate to the lowest supported audio rate."""
        for testrate in [44100]:
            if self.valid_test(device, testrate):
                return testrate
        print("SOMETHING'S WRONG! I can't figure out how to use DEV", device)
        return None

    def valid_test(self, device, rate=44100):
        """given a device ID and a rate, return TRUE/False if it's valid."""
        try:
            self.info = self.p.get_device_info_by_index(device)
            if not self.info["maxInputChannels"] > 0:
                return False
            stream = self.p.open(format=pyaudio.paInt16, channels=1,
                                 input_device_index=device, frames_per_buffer=self.chunk,
                                 rate=int(self.info["defaultSampleRate"]), input=True)
            stream.close()
            return True
        except:
            return False

    def valid_input_devices(self):
        """
        See which devices can be opened for microphone input.
        call this when no PyAudio object is loaded.
        """
        mics = []
        for device in range(self.p.get_device_count()):
            if self.valid_test(device):
                mics.append(device)
        if len(mics) == 0:
            print("no microphone devices found!")
        else:
            print("found %d microphone devices: %s" % (len(mics), mics))
        return mics

    ### SETUP AND SHUTDOWN ////////////////////////////////////////////////////////////////////////////////////////////

    def initiate(self):
        """run this after changing settings (like rate) before recording"""
        if self.device is None:
            self.device = self.valid_input_devices()[0]  # pick the first one
        if self.rate is None:
            self.rate = self.valid_low_rate(self.device)
        self.chunk = int(self.rate / self.updatesPerSecond)  # hold one tenth of a second in memory
        if not self.valid_test(self.device, self.rate):
            print("guessing a valid microphone device/rate...")
            self.device = self.valid_input_devices()[0]  # pick the first one
            self.rate = self.valid_low_rate(self.device)
        self.datax = np.arange(self.chunk) / float(self.rate)
        msg = 'recording from "%s" ' % self.info["name"]
        msg += '(device %d) ' % self.device
        msg += 'at %d Hz' % self.rate
        print(msg)


    def close(self):
        """gently detach from things."""
        print(" -- sending stream termination command...")
        self.keepRecording = False  # the threads should self-close
        while (self.t.isAlive()):  # wait for all threads to close
            time.sleep(.1)
        self.stream.stop_stream()
        self.p.terminate()

    ### STREAM HANDLING

    def stream_readchunk(self):
        """reads some audio and re-launches itself"""
        try:

            self.data0 = np.fromstring(self.stream.read(self.chunk), dtype=np.int16)
            nyq = 44100 / 2.0
            n1 = 1000 / nyq
            n2 = 2000 / nyq
            n3 =  1000/nyq
            n4 = 2000/nyq
            b, a = signal.butter(5, [n1, n2], btype='bandpass', analog=False, output='ba')
            b2, a2 = signal.butter(5, [n3, n4], btype='bandstop', analog=False, output='ba')
            #self.data = signal.lfilter(b, a, signal.lfilter(b, a, self.data0))
            self.data1 = signal.lfilter(b, a, self.data0)
            self.data2 = signal.lfilter(b2, a2, self.data0)
            self.data =self.data1 +self.data2 *self.go.hanna
            print (self.go.hanna)
            #print len(self.data)
            self.fftx, self.fft = getFFT(self.data, self.rate)
            #print len(self.fftx) ,len(self.fft) ,self.rate
            #data = self.stream.read(self.chunk)
            #self.stream.write(data, self.chunk)


        except Exception as E:
            print(" -- exception! terminating...")
            print(E, "\n" * 5)
            self.keepRecording = False
        if self.keepRecording:
            self.stream_thread_new()
        else:
            self.stream.close()
            self.p.terminate()
            print(" -- stream STOPPED")
        self.chunksRead += 1

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
        self.stream = self.p.open(format=pyaudio.paInt16, channels=1,
                                  rate=self.rate, input=True,output=True, frames_per_buffer=self.chunk)
        self.stream2 = self.p.open(format=self.p.get_format_from_width(2),channels=1,rate=self.rate,
                        input=True,output=True,frames_per_buffer=self.chunk)


        self.stream_thread_new()

    def set_go_instance(self,go):
        self.go = go

#test that we can acces go instance
    def print_go(self):
        print self.go
if __name__ == "__main__":
    print ("HIIIII")
    ear = SWHear(updatesPerSecond=10)  # optinoally set sample rate here
    ear.stream_start()  # goes forever
    lastRead = ear.chunksRead
    while True:
        while lastRead == ear.chunksRead:
            time.sleep(.01)
        print(ear.chunksRead, len(ear.data))
        lastRead = ear.chunksRead
    print("DONE")