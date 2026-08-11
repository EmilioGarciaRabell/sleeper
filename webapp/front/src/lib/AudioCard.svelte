<script>
  // Expose props so you can pass dynamic data later
  let { time,text,transcript } = $props();

  let file_text = $state.raw();
  
  $effect(() => {
    async function showTranscript(){
    const url = import.meta.env.VITE_BACKEND_URL
    console.log(url)
    const postURL = `${url}/transcription/${transcript}.txt`;
    try{
      const response = await fetch(postURL)
      console.log(response)
      if (!response.ok){
          console.log("error")
          return 
      }
      console.log(response)
      const res = await response.text()
      file_text = res
      console.log(res)
      
    }catch (error){
      console.log("errrorrr")
      console.log(error)
    }
    }
    showTranscript()
    
  });
  
</script>

<div class="audio-card">
  <span class="time">{time}</span>
  <p>Post Number: {text}</p>
  <p>Transcription: {file_text}</p>
  
  <!-- Custom audio player area to build out -->
  <div class="player-stub">
    <button>▶</button>
    <div class="track">
    </div>
  </div>
</div>

<style>
  .audio-card {
    background: #ffffff;
    border-radius: 10px;
    padding: 20px;
    border: 1px solid #eae6df;
    margin-bottom: 16px;
  }
  .time {
    font-size: 11px;
    color: #8c857b;
    font-weight: 600;
  }
  p {
    margin: 10px 0 16px 0;
    color: #2c2925;
  }
  .player-stub {
    display: flex;
    align-items: center;
    gap: 10px;
  }
  .track {
    flex-grow: 1;
    height: 4px;
    background: #e6e2db;
    border-radius: 2px;
  }
  button {
    background: #1a1a1a;
    color: white;
    border: none;
    border-radius: 50%;
    width: 28px;
    height: 28px;
    cursor: pointer;
  }
</style>