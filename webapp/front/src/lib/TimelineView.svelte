<script>
  import AudioCard from './AudioCard.svelte';
</script>

<div class="timeline">
  <div class="date-header">SUNDAY, JULY 12, 2026</div>

  <!-- Drop in your custom AudioCards here -->
  <AudioCard time="10:24 AM" text="Met with the team to design the SPI wiring interface." />
  <AudioCard time="9:15 AM" text="Remember to check the engine oil on Friday morning." />
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