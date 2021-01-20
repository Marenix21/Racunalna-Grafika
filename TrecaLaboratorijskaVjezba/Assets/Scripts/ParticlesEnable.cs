using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParticlesEnable : MonoBehaviour
{

    public float waitFor;

    private IEnumerator coroutine;



    private IEnumerator MakeVisible()
    {
        yield return new WaitForSeconds(waitFor);
        ParticleSystem ps = this.GetComponent<ParticleSystem>();
        ps.Stop();
        ps.Play();
        if (this.GetComponent<Animator>() != null)
        {
            this.GetComponent<Animator>().SetBool("Restart", true);
        }
        var emission = ps.emission;
        emission.enabled = true;
    }



    void Start()
    {
        ParticleSystem ps = this.GetComponent<ParticleSystem>();
        var emission = ps.emission;
        emission.enabled = false;
    }



    public void TurnOn()
    {
        coroutine = MakeVisible();
        StartCoroutine(coroutine);
    }
}
