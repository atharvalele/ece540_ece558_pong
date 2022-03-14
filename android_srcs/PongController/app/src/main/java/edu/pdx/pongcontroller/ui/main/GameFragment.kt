package edu.pdx.pongcontroller.ui.main

import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import edu.pdx.pongcontroller.MainActivity
import edu.pdx.pongcontroller.databinding.GameFragmentBinding

class GameFragment : Fragment() {

    // Create binding
    private var _binding: GameFragmentBinding? = null
    private val binding get() = _binding!!

    companion object {
        fun newInstance() = GameFragment()
    }

    private lateinit var viewModel: GameViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = GameFragmentBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        viewModel = ViewModelProvider(this).get(GameViewModel::class.java)

        binding.downButton.setOnClickListener{
            (activity as MainActivity).sendUDPMessage("1")
        }

        binding.upButton.setOnClickListener {
            (activity as MainActivity).sendUDPMessage("0")
        }

        // TODO: Use the ViewModel
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    override fun onStart() {
        super.onStart()
        arguments?.let {
            val args = GameFragmentArgs.fromBundle(it)
            Log.d("WAIT", "Got player names ${args.p1Name}, ${args.p2Name}")

            binding.p1Name.text = args.p1Name
            binding.p2Name.text = args.p2Name
        }
    }

    fun updateScoreBoard(plScore: String, p2Score: String) {
        binding.p1Score.text = plScore
        binding.p2Score.text = p2Score
    }
    // Fragment Navigation
    interface OnFragmentInteractionListener {
        fun onFragmentInteraction(uri: Uri)
    }

}