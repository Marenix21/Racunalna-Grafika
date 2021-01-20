using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PointLightScript : MonoBehaviour
{
    private int turn = 1;
    void Update()
    {
        if(turn % 20 == 0)
        {
            float x = UnityEngine.Random.Range(-1f, 1f);
            this.GetComponent<Light>().range += x;
            if(this.GetComponent<Light>().range > 200f)
            {
                this.GetComponent<Light>().range = 200;
            } else if(this.GetComponent<Light>().range < 100f)
            {
                this.GetComponent<Light>().range = 100f;
            }
            turn = 1;
        } else if (turn % 10 == 0)
        {
            float x = UnityEngine.Random.Range(-0.05f, 0.05f);
            this.GetComponent<Light>().intensity += x;
            if (this.GetComponent<Light>().intensity > 3.5f)
            {
                this.GetComponent<Light>().intensity = 3.5f;
            } else if(this.GetComponent<Light>().intensity < 2.5f)
            {
                this.GetComponent<Light>().intensity = 2.5f;
            }
        }
        turn++;
    }
}
