#https://github.com/swharden/Python-GUI-examples/tree/master/2016-07-37_qt_audio_monitor
from PyQt4 import QtGui,QtCore
from PyQt4.uic import loadUiType
import sys
import ui_main
import numpy as np
from scipy import signal
import pyqtgraph
import SWHear
from bluetooth import *
#import pyaudio
import sounddevice as sd

Ui_MainWindow, QMainWindow = loadUiType('ui_main.ui')

class ExampleApp(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        pyqtgraph.setConfigOption('background', 'w') #before loading widget
        super(ExampleApp, self).__init__(parent)
        self.setupUi(self)
        self.grFFT.plotItem.showGrid(True, True, 0.7)
        self.grPCM.plotItem.showGrid(True, True, 0.7)
        self.maxFFT=0
        self.maxPCM=0
        self.norm = 1
        self.norm2 = 1
        self.norm3 = 1
        self.norm4 = 1
        self.ear = SWHear.SWHear(rate=44100,updatesPerSecond=20)

        self.ear.stream_start()
        self.ear.set_go_instance(self)
        self.ear.print_go()
        self.horizontalSlider.valueChanged.connect(self.valuechange)
        self.horizontalSlider_2.valueChanged.connect(self.valuechange)
        self.horizontalSlider_3.valueChanged.connect(self.valuechange)
        self.horizontalSlider_4.valueChanged.connect(self.valuechange)
       
        self.pushButton.clicked.connect(self.blue)

        self.flag=0

    def valuechange (self ):
        z = float(self.horizontalSlider.value())/100
        self.label_7.setText(str(z) + 'dB')
        self.norm = float(z)
        z2 = float(self.horizontalSlider_2.value()) / 100
        self.label_8.setText(str(z2) + 'dB')
        self.norm2 = float(z2)
        z3 = float(self.horizontalSlider_3.value()) / 100
        self.label_9.setText(str(z3) + 'dB')
        self.norm3 = float(z3)
        z4 = float(self.horizontalSlider_4.value()) / 100
        self.label_10.setText(str(z) + 'dB')
        self.norm4 = float(z)

    #def valuechange2(self):
    #def valuechange3(self):
    #def valuechange4(self):
    def blue(self):
        self.server_socket = BluetoothSocket(RFCOMM)

        port = 1
        self.server_socket.bind(("", PORT_ANY))
        self.server_socket.listen(1)
        self.label_11.setText('Listening for Clients')
        self.client_socket, address = self.server_socket.accept()
        self.label_11.setText( 'Accepted connection from '+str( address))
        self.flag=1



    def update(self):
        #self.ear.stream_readchunk()
        if self.flag ==1 :

            #if self.client_socket.recv(1024) != "":
            String = self.client_socket.recv(1024)
            #print  String
            dataArray = String.split(',')
            f = int(dataArray[0])
            a = int(dataArray[1])
            if f == 1:
                self.horizontalSlider.setValue(a)
                self.valuechange()
                #print ("1")
            elif f == 2:
                self.horizontalSlider_2.setValue(a)
                self.valuechange()
                #print ("2")
            elif f == 3:
                self.horizontalSlider_3.setValue(a)
                self.valuechange()
                #print ("2")
            elif f == 4:
                self.horizontalSlider_4.setValue(a)
                self.valuechange()
                #print ("2")
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
     #start with something
    form.update()
    app.exec_()
    #while True:
        #form.update()

        #form.ear.stream_readchunk()
    print("DONE")
