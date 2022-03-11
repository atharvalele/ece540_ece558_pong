package edu.pdx.pongcontroller.ui.main

import androidx.lifecycle.ViewModelProvider
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import edu.pdx.pongcontroller.R

import android.net.Uri
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
        // TODO: Use the ViewModel
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    // Fragment Navigation
    interface OnFragmentInteractionListener {
        fun onFragmentInteraction(uri: Uri)
    }
}