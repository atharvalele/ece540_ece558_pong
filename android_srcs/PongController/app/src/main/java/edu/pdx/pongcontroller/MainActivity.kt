package edu.pdx.pongcontroller

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.net.Uri
import android.os.StrictMode
import android.util.Log
import androidx.navigation.NavHostController
import androidx.navigation.Navigation
import androidx.navigation.findNavController
import androidx.navigation.fragment.findNavController
import edu.pdx.pongcontroller.ui.main.*
import java.io.IOException
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

open class MainActivity : AppCompatActivity(), GameFragment.OnFragmentInteractionListener {
    // UDP Connection Settings
    private val UDP_IP_ADDR: String = "192.168.4.1"
    private val UDP_SEND_PORT: Int = 2000
    private val UDP_RECEIVE_PORT: Int = 2001

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_activity)

        // Start UDP Thread
        val UDPThread = Thread(udp_DataArrival(this))
        UDPThread.start()
    }

    override fun onFragmentInteraction(uri: Uri) {
        TODO("Not yet implemented")
    }

    // Helper functions
    fun parsePacket(packet: String) {
        val navController = findNavController(R.id.gameNavHostFragment)
        val parts = packet.split(",")
        Log.d("UDP", "Split message")


        // PacketType Controller, Command, Start
        when(parts[0]) {
            "S" -> {
                // Create action to move to next, pass names as args
                val action: WaitFragmentDirections.ActionWaitFragmentToGameFragment =
                    WaitFragmentDirections.actionWaitFragmentToGameFragment()
                action.p1Name = parts[1]
                action.p2Name = parts[2]
                navController.navigate(action)

            }
            "B" -> {
                var nav = supportFragmentManager.findFragmentById(R.id.gameNavHostFragment)
                val gameFragment = nav?.childFragmentManager?.fragments?.get(0)

                (gameFragment as GameFragment)?.updateScoreBoard(parts[1], parts[2])
                Log.d("UDP", "Received B")
            }
            "G" ->{
//                var nav = supportFragmentManager.findFragmentById(R.id.gameNavHostFragment)
//                val gameOverFragment = nav?.childFragmentManager?.fragments?.get(0)
//
//                (gameOverFragment as GameOverFragment)?.gameOver()
                val action: GameFragmentDirections.ActionGameFragmentToGameOverFragment =
                    GameFragmentDirections.actionGameFragmentToGameOverFragment(parts[2])
                navController.navigate(action)
            }
        }
    }

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
            val sendPacket = DatagramPacket(
                sendData,
                sendData.size,
                InetAddress.getByName(UDP_IP_ADDR),
                UDP_SEND_PORT
            )
            socket.send(sendPacket)
        } catch (e: IOException) {
            Log.e("MainActivity", "IOException: " + e.message)
        }
    }
}

// UDP Class
class udp_DataArrival: Runnable {

    // UDP Connection Settings
    private val UDP_IP_ADDR: String = "192.168.4.1"
    private val UDP_SEND_PORT: Int = 2000
    private val UDP_RECEIVE_PORT: Int = 2001

    // Reference to MainActivity instance
    lateinit var mainActivityRef: MainActivity

    constructor(mainAct: MainActivity) {
        mainActivityRef = mainAct
    }

    // UDP Rx function
    fun receiveUDP() {
        val buffer = ByteArray(128)
        var socket: DatagramSocket? = null
        try {
            // Rx data
            socket = DatagramSocket(UDP_RECEIVE_PORT)
            socket.broadcast = true
            val packet = DatagramPacket(buffer, buffer.size)
            socket.receive(packet)

            // Packet Data
            val packetData = packet.data.decodeToString(0, packet.length)
            Log.d("UDP", "open fun receiveUDP packet received = $packetData")

            // Pass packet to main UI thread for parsing
            // Otherwise causes an exception
            mainActivityRef.runOnUiThread { mainActivityRef.parsePacket(packet.data.decodeToString()) }
        } catch (e: Exception) {
            Log.d("UDP", "open fun receiveUDP catch exception." + e.toString())
            e.printStackTrace()
        } finally {
            socket?.close()
        }
    }

    // Override run from Runnable
    override fun run() {
        Log.d("UDP", "${Thread.currentThread()} Runnable Thread Started.")
        while (true){
            receiveUDP()
        }
    }
}