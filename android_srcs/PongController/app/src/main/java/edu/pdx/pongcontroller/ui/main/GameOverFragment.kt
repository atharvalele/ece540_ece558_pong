package edu.pdx.pongcontroller.ui.main

import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import edu.pdx.pongcontroller.R
import edu.pdx.pongcontroller.databinding.GameOverFragmentBinding
import edu.pdx.pongcontroller.databinding.WaitFragmentBinding

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
        viewModel = ViewModelProvider(this).get(GameOverViewModel::class.java)
        // TODO: Use the ViewModel
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}