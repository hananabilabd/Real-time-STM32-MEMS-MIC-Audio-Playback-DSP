import sounddevice as sd
import numpy as np
duration = 50  # seconds

def callback(indata, outdata, frames, time, status):
    if status:
        print(status)
    print type(indata)
    print indata.shape
    outdata[:] = indata
    #print len(outdata)
stream=sd.Stream(channels=1)
stream.start()
while True:
        i,c=stream.read(frames=800)
        #print type(i)
        #v=np.array(i)
        #o = v.astype('float32')
        #i2=np.array(i,dtype='float32')
        stream.write(i)

        #sd.sleep(int(duration * 1000))