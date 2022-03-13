package edu.pdx.pongcontroller.ui.main

import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.navigation.Navigation
import androidx.navigation.fragment.findNavController
import edu.pdx.pongcontroller.MainActivity
import edu.pdx.pongcontroller.R
import edu.pdx.pongcontroller.databinding.MainFragmentBinding
import edu.pdx.pongcontroller.databinding.WaitFragmentBinding

class WaitFragment : Fragment() {
    // Create binding
    private var _binding: WaitFragmentBinding? = null
    private val binding get() = _binding!!

    // Player Name placeholder
    private lateinit var playerName: String

    companion object {
        fun newInstance() = WaitFragment()
    }

    private lateinit var viewModel: WaitViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = WaitFragmentBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProvider(this).get(WaitViewModel::class.java)
        // TODO: Use the ViewModel
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onStart() {
        super.onStart()
        arguments?.let {
            val args = WaitFragmentArgs.fromBundle(it)
            playerName = args.tempName
            Log.d("WAIT", "Got player name $playerName")
        }

        (activity as MainActivity).sendUDPMessage(playerName)
    }

    fun navigateToGame() {
        /*val action: MainFragmentDirections.ActionMainFragmentToWaitFragment =
            MainFragmentDirections.actionMainFragmentToWaitFragment()
        action.setTempName(playerName)

        (activity as MainActivity).sendUDPMessage(playerName)

        // Navigate to the next fragment
        Navigation.findNavController(it).navigate(action)*/

        val action = WaitFragmentDirections.actionWaitFragmentToGameFragment()
        Navigation.findNavController(binding.root).navigate(action)
    }
}