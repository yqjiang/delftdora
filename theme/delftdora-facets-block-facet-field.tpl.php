<?php

/**
 * @file
 * Renders a single facet inside the facets block.
 */
?>
<div class="delftdora-facet">
  <h2><?php print $label; ?></h2>
  <ol class="filterlist">
    <?php foreach ($options as $index => $option) :?>
      <?php $hidden = $index >= $display_limit; ?>
      <?php $style = $hidden ? 'display: none;' : ''; ?>
      <li class="<?php print implode($option['classes']); ?>" style="<?php print $style; ?>">
        <a rel="nofollow" href="<?php print $option['url']; ?>">
          <span></span>
          <?php print $option['label']; ?>
        </a>
        <?php print $option['count']; ?>
      </li>
    <?php endforeach; ?>
  </ol>
  <?php if ($show_more): ?>
  <div class="show-more <?php print implode($classes); ?>">
    <b data-show-text="<?php print $show_text; ?>" data-hide-text="<?php print $hide_text; ?>"><?php print $show_text; ?></b>
  </div>
  <?php endif; ?>
</div>
