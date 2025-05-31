---
layout: default
title: Overview
nav_order: 1
---

# Lord of the Strings

## Overview
The **Lord of the Strings** is more than just a simple guitar strummer. This robot also features palm muting—a technique used by advanced guitarists to dampen strings and enhance rhythmic articulation. The integration of both palm muting and strumming actions on an acoustic guitar introduces unique mechanical and timing challenges, including precise servo control with PWM and synchronization between strumming and muting.

Our original goal was to program a microcontroller to control both the strumming speed and style. The current implementation allows users to switch between pre-programmed strumming styles and toggle palm muting using external buttons. To achieve precise strumming patterns, we used the TPM (Timer/PWM Module) to drive the servos and the PIT (Periodic Interrupt Timer) to schedule strumming and muting actions with precise timing.

## Pages
[Overview]({{ site.baseurl }}/) |
[Media/Demo]({{ site.baseurl }}/media) |
[Technical Approach]({{ site.baseurl }}/technical) |
[Testing & Debugging]({{ site.baseurl }}/testing) |
[Teamwork]({{ site.baseurl }}/team) |
[Future Improvements]({{ site.baseurl }}/improv) |
[Outside Resources]({{ site.baseurl }}/resources) |

## Video    
<div style="position: relative; padding-bottom: 56.25%; height: 0; overflow: hidden; max-width: 100%;">
  <iframe 
    src="https://www.youtube.com/embed/4oN9tlPXdls?si=k9DWxZWXGWzzSCU3"
    style="position: absolute; top: 0; left: 0; width: 100%; height: 100%;"
    frameborder="0"
    allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
    allowfullscreen
    title="YouTube video player"
    referrerpolicy="strict-origin-when-cross-origin">
  </iframe>
</div>

## System Diagram
![System Diagram](images/System%20Diagram.jpg)
