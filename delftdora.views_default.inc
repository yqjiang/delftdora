<?php

/**
 * @file
 * Defualt views that invoke all the views under views/.
 */

/**
 * Implements hook_views_default_views().
 */
function delftdora_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'delftdora') . '/views', '/\.view/');
  foreach ($files as $absolute => $file) {
    require $absolute;
    if (isset($view)) {
      $views[$file->name] = $view;
    }
  }
  return $views;
}
