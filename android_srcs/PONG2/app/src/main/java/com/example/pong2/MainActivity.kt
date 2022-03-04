package com.example.pong2

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.fragment.app.FragmentManager
import com.example.pong2.ui.main.MainFragment
import com.example.pong2.ui.main.placeholder.Fragment1

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.main_fragment)
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.nav_menu, menu)
        return true
    }

    val fragment1 = Fragment1()

    /*supportFragmentManager.beginTransaction().apply{
    replace(R.id.tvFragFirst, fragment1)
    commit()
}*/
    /*override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when(item.itemId){
            R.id.action_rules -> Toast.makeText(this, "Rules selected", Toast.LENGTH_SHORT).show()
            R.id.action_settings -> Toast.makeText(this, "Settings selected", Toast.LENGTH_SHORT).show()
        }
        return super.onOptionsItemSelected(item)
    }*/
}