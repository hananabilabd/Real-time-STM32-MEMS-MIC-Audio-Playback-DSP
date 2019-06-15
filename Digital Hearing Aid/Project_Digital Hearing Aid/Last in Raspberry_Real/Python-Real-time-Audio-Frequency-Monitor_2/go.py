#https://github.com/swharden/Python-GUI-examples/tree/master/2016-07-37_qt_audio_monitor
from PyQt4 import QtGui,QtCore
from PyQt4.uic import loadUiType
import sys
import ui_main
import numpy as np
from scipy import signal
import pyqtgraph
import SWHear
import time
import pyaudio
import sounddevice as sd
Ui_MainWindow, QMainWindow = loadUiType('ui_main.ui')

class ExampleApp(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        self.app = QtGui.QApplication(sys.argv)
        pyqtgraph.setConfigOption('background', 'w') #before loading widget
        super(ExampleApp, self).__init__(parent)
        self.setupUi(self)
        self.grFFT.plotItem.showGrid(True, True, 0.7)
        self.grPCM.plotItem.showGrid(True, True, 0.7)
        self.maxFFT=0
        self.maxPCM=0
        self.ear = SWHear.SWHear(rate=44100,updatesPerSecond=20)
        self.ear.stream_start()
        self.ear.set_go_instance(self)
        self.ear.print_go()
        self.verticalSlider.valueChanged.connect(self.valuechange)
        self.pushButton.clicked.connect(self.test)
        self.norm= 1
        self.hanna = 1

    def valuechange (self ):
        z = str(self.verticalSlider.value())
        self.label_3.setText(z + 'dB')
        self.norm = float(z) /30

        #self.zico = np.concatenate((self.yfourier[(self.x < 900)] * 0.3, self.yfourier[(self.x>900)]*0.3), axis=0)
        for i in range (0,1100):
            if self.ear.fftx[i] < 1000:
                self.ear.fft[i] = self.norm * self.ear.fft[i]
        #self.zico =yfourier1
    def test(self):
        nyq = 41000 / 2.0
        n1 = 100 / nyq
        n2 = 500 / nyq
        b, a = signal.butter(3, [n1, n2], btype='bandpass', analog=False, output='ba')
        out = signal.lfilter(b, a, signal.lfilter(b, a, impulse))


    def update(self):
        if not self.ear.data is None and not self.ear.fft is None:
            pcmMax=np.max(np.abs(self.ear.data))
            if pcmMax>self.maxPCM:
                self.maxPCM=pcmMax
                self.grPCM.plotItem.setRange(yRange=[-pcmMax,pcmMax])
            if np.max(self.ear.fft)>self.maxFFT:
                self.maxFFT=np.max(np.abs(self.ear.fft))
                #self.grFFT.plotItem.setRange(yRange=[0,self.maxFFT])
                self.grFFT.plotItem.setRange(yRange=[0,1])
            self.pbLevel.setValue(1000*pcmMax/self.maxPCM)
            #print len(self.ear.fft),len(self.ear.fftx)

            pen=pyqtgraph.mkPen(color='b')
            self.grPCM.plot(self.ear.datax,self.ear.data,pen=pen,clear=True)
            pen=pyqtgraph.mkPen(color='r')
            self.grFFT.plot(self.ear.fftx,self.ear.fft/self.maxFFT,pen=pen,clear=True)
        QtCore.QTimer.singleShot(1, self.update) # QUICKLY repeat

if __name__=="__main__":
    app = QtGui.QApplication(sys.argv)
    form = ExampleApp()
    form.show()
    form.update() #start with something
    # ear = SWHear(updatesPerSecond=10)  # optinoally set sample rate here
    # form.ear.stream_start()  # goes forever
    # lastRead = form.ear.chunksRead
    # while True:
    #     while lastRead == form.ear.chunksRead:
    #         time.sleep(.01)
    #     print(form.ear.chunksRead, len(form.ear.data))
    #     lastRead = form.ear.chunksRead
    app.exec_()
    print("DONE")