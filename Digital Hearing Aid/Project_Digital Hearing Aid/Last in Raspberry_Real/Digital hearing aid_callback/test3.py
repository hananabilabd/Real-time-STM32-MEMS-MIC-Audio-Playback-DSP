import sounddevice as sd
duration = 50  # seconds

def callback(indata, outdata, frames, time, status):
    if status:
        print(status)
    i= indata
    #print type(i)
    #print i.shape
    outdata[:]=i
    #print outdata.shape

stream=sd.Stream(channels=1,blocksize=4000,samplerate=44100,callback=callback)
stream.start()
while True:
    stream


    #sd.sleep(int(duration * 1000))
