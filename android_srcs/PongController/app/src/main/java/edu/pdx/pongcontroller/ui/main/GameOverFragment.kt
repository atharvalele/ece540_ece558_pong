package edu.pdx.pongcontroller.ui.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import edu.pdx.pongcontroller.MainActivity
import edu.pdx.pongcontroller.databinding.GameOverFragmentBinding

class GameOverFragment : Fragment() {
    // Create binding
    private var _binding: GameOverFragmentBinding? = null
    private val binding get() = _binding!!

    companion object {
        fun newInstance() = GameOverFragment()
    }

    private lateinit var viewModel: GameOverViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        _binding = GameOverFragmentBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        arguments?.let {
            val args = GameOverFragmentArgs.fromBundle(it)
            binding.winnerText.text = "${args.winnerName}  won!"
        }
        viewModel = ViewModelProvider(this).get(GameOverViewModel::class.java)
        // TODO: Use the ViewModel

        binding.startButton.setOnClickListener {
            (activity as MainActivity).sendUDPMessage("SO")
        }

        binding.replayButton.setOnClickListener {
            (activity as MainActivity).sendUDPMessage("RST")

        }
    }

    fun callParentMethod() {
        requireActivity().onBackPressed()
    }




    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

}