package edu.pdx.pongcontroller.ui.main

import android.content.Context
import android.os.Build
import android.os.Bundle
import android.os.Vibrator
import android.os.VibratorManager
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.core.content.ContextCompat.getSystemService
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.Navigation
import edu.pdx.pongcontroller.databinding.MainFragmentBinding

class MainFragment : Fragment() {

    // Create binding
    private var _binding: MainFragmentBinding? = null
    private val binding get() = _binding!!

    companion object {
        fun newInstance() = MainFragment()
    }

    private lateinit var viewModel: MainViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = MainFragmentBinding.inflate(inflater, container, false)
        return binding.root
    }



    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProvider(this).get(MainViewModel::class.java)
        // TODO: Use the ViewModel

        // Set button onClickListener
        binding.goButton.setOnClickListener {
            if (binding.nameField.text.isNotEmpty()) {
                val playerName = binding.nameField.text.toString()
                // Accept the name
                viewModel.setPlayerName(playerName)

                // Send args to next fragment
                val action: MainFragmentDirections.ActionMainFragmentToWaitFragment =
                    MainFragmentDirections.actionMainFragmentToWaitFragment()
                action.setTempName(playerName)


                // Navigate to the next fragment\
                Navigation.findNavController(it).navigate(action)
            } else {
                Toast.makeText(activity, "Name cannot be empty", Toast.LENGTH_LONG).show()
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

}