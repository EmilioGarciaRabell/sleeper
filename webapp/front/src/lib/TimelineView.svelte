
<script>
  import AudioCard from './AudioCard.svelte';

  let posts = $state.raw(null)

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
      const res = await response.json()
      posts = res
      console.log(res)
      
    }catch (error){
      console.log("errrorrr")
      console.log(error)
    }
  }

  $effect(() => {
    getAllPost()
  });

</script>

<div class="timeline">
  <div class="date-header">SUNDAY, JULY 12, 2026
  </div>
  {#if posts} 
    {#each posts as post}
      {#if post}
        <AudioCard time={post.date} text={post.id} transcript={post.transcription_file_location}/>
      {/if}
    {/each}
  {/if}
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
