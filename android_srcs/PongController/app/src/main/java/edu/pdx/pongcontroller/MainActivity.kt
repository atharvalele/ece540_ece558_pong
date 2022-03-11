package edu.pdx.pongcontroller

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.fragment.app.FragmentActivity
import android.net.Uri
import android.os.StrictMode
import android.util.Log
import android.widget.Toast
import edu.pdx.pongcontroller.ui.main.GameFragment
import edu.pdx.pongcontroller.ui.main.MainFragment
import java.io.IOException
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

open class MainActivity : AppCompatActivity(), GameFragment.OnFragmentInteractionListener {
    // UDP Connection Settings
    private val UDP_IP_ADDR: String = "192.168.4.1"
    private val UDP_SEND_PORT: Int  = 2000
    private val UDP_RECEIVE_PORT: Int = 2001

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)

        // Start UDP Thread
        val UDPThread = Thread(udp_DataArrival())
        UDPThread.start()
    }

    override fun onFragmentInteraction(uri: Uri) {
        TODO("Not yet implemented")
    }

    // Helper functions

    // Send Data over UDP
    fun sendUDPMessage(message: String) {
        // Hack Prevent crash (sending should be done using an async task)
        val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)
        try {
            // Open a port to send the package
            val socket = DatagramSocket()
            socket.broadcast = true
            // Get the message from function parameters and make it into a byte array
            val sendData = message.toByteArray()
            // Craft the packet with the parameters
            val sendPacket = DatagramPacket(sendData, sendData.size, InetAddress.getByName(UDP_IP_ADDR), UDP_SEND_PORT)
            socket.send(sendPacket)
        } catch (e: IOException) {
            Log.e("MainActivity", "IOException: " + e.message)
        }
    }

    open fun receiveUDP() {
        val buffer = ByteArray(128)
        var socket: DatagramSocket? = null
        try {
            // Keep a socket open to listen to all the UDP trafic that is destined for this port
            // socket = DatagramSocket(UDP_RECEIVE_PORT, InetAddress.getByName(UDP_IP_ADDR))

            socket = DatagramSocket(UDP_RECEIVE_PORT)
            socket.broadcast = true
            val packet = DatagramPacket(buffer, buffer.size)
            socket.receive(packet)
            Log.d("UDP", "open fun receiveUDP packet received = " + packet.data.decodeToString(0, packet.length))
            //Toast.makeText(this, "Message RX: ${packet.data}", Toast.LENGTH_LONG).show()

        } catch (e: Exception) {
            Log.d("UDP", "open fun receiveUDP catch exception." + e.toString())
            e.printStackTrace()
        } finally {
            socket?.close()
        }
    }
}

class udp_DataArrival: Runnable, MainActivity() {
    public override fun run() {
        Log.d("UDP", "${Thread.currentThread()} Runnable Thread Started.")
        while (true){
            receiveUDP()
        }
    }
}