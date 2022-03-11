package edu.pdx.pongcontroller.ui.main

import android.util.Log
import androidx.lifecycle.ViewModel

class MainViewModel : ViewModel() {
    private lateinit var playerName: String
    // TODO: Implement the ViewModel
    fun setPlayerName(name: String) {
        Log.d("MainViewModel", "Player Name is $name")
        playerName = name
    }
}