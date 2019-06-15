import sounddevice as sd
duration = 50  # seconds
import time
import threading
def callback(indata, outdata, frames, time, status):
    if status:
        print(status)
    i= indata
    #print type(i)
    #print i.shape
    outdata[:]=i
    #print outdata.shape
    #print ("callback")
i=0
def stream_thread_new():
    t = threading.Thread(target=read)
    t.start()

def read():

    stream


    #sd.sleep(int(duration * 1000))
    time.sleep(0.01)
    stream_thread_new()

stream=sd.Stream(channels=1,blocksize=4096,samplerate=44100,callback=callback)
stream.start()



read()