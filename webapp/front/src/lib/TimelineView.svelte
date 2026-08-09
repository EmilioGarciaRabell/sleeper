<script>
  import AudioCard from './AudioCard.svelte';
// import { BACKEND_URL } from '$app/env/public'

  let posts = $state([])

  async function getAllPost(){
    const url = import.meta.env.VITE_BACKEND_URL
    console.log(url)
    const postURL = `${url}/api/allposts`;
    try{
      const response = await fetch(postURL)
      console.log(response)
      if (!response.ok){
        console.log("error")
        return 
      }
      console.log(response)
      // const res = await response.json()
      // posts = res
      // console.log(res)
      
    }catch (error){
      console.log("errrorrr")
      console.log(error)
    }
  }

</script>



<div class="timeline">
  <div class="date-header">SUNDAY, JULY 12, 2026</div>
  <!-- Drop in your custom AudioCards here -->

  <!-- {#if posts} 
    {#each posts as post}
      <AudioCard time={post.date} text={post.id} />
    {/each}
    <AudioCard time="9:15 AM" text="Remember to check the engine oil on Friday morning." /> -->
    <!-- <p>The API endpoint is: {}</p> -->
  <!-- {/if} -->
  <button onclick={getAllPost}>Fetch Data</button>
</div>

<style>
  .timeline {
    max-width: 720px;
    margin: 0 auto;
    padding: 40px 20px;
  }
  .date-header {
    font-size: 11px;
    font-weight: 600;
    color: #8c857b;
    margin-bottom: 20px;
    letter-spacing: 1px;
  }
</style>