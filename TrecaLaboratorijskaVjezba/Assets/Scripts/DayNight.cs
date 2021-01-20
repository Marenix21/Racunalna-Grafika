using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DayNight : MonoBehaviour
{

    private IEnumerator coroutine;
    float intensity = 0;
    Color color = new Color(1f, 135f / 255, 75f / 255, 1f);
    int y = -90;



    private IEnumerator StartAnimation()
    {
        Color col = new Color(0f, 6f / 255, 9f / 255, 0f);
        while (true)
        {
            yield return new WaitForSeconds(0.33f);
            y++;
            if(y > 90)
            {
                y = -269;
            }
            if (y >= -195 && y <= -176)
            {
                this.GetComponent<Light>().intensity -= 0.05f;
                intensity = this.GetComponent<Light>().intensity;
            }else if(y >= -4 && y <= 15)
            {
                this.GetComponent<Light>().intensity += 0.05f;
                intensity = this.GetComponent<Light>().intensity;
            }
            if(y >= 0 && y < 20)
            {
                this.GetComponent<Light>().color += col;
                color = this.GetComponent<Light>().color;
            } else if(y > -200 && y <= -180)
            {
                this.GetComponent<Light>().color -= col;
                color = this.GetComponent<Light>().color;
            }
            this.transform.Rotate(1f, 0f, 0f);

        }
    }



    public void StartRotation()
    {
        coroutine = StartAnimation();
        StartCoroutine(coroutine);
    }



    public void StopRotation()
    {
        StopCoroutine(coroutine);
    }



    public void TurnOn()
    {
        this.GetComponent<Light>().intensity = intensity;
        this.GetComponent<Light>().color = color;
    }



    public void TurnOff()
    {
        this.GetComponent<Light>().intensity = 0;
    }
}
