<?php

/**
 * @file
 * Theme functions.
 */

/**
 * Implements theme_hook().
 */
function theme_delftdora_facets_block(array &$variables) {
  $path = drupal_get_path('module', 'delftdora');
  drupal_add_css("$path/css/delftdora.solr.css", array('type' => 'file'));
  $facets = array();
  if (isset($variables['solr']->islandoraSolrResult['facet_counts']['facet_fields'])) {
    $facet_fields = $variables['solr']->islandoraSolrResult['facet_counts']['facet_fields'];
    foreach ($facet_fields as $facet_field => $options) {
      // Skip facets without options.
      if (count($options) === 0) {
        continue;
      }
      $facets[$facet_field] = array(
        '#theme' => 'delftdora_facets_block_facet_field',
        '#solr' => $variables['solr'],
        '#field' => $facet_field,
        '#options' => $options,
      );
    }
    return '<div id="filters">' . drupal_render($facets) . '</div>';
  }
}

function theme_delftdora_front_search_block(array &$variables) {
  $path = drupal_get_path('module', 'delftdora');
  drupal_add_css("$path/css/delftdora.frontsearch.css", array('type' => 'file'));
  module_load_include('inc', 'islandora_solr','includes/blocks');
  $search = drupal_get_form('islandora_solr_simple_search_form');
  $html = '<div id="searchboxwrapper"><h2>Search</h2>';
  $html .= '<div id="searchbox" class="">';
  $html .= drupal_render($search);
  $html .= '</div>';
  $html .= '<h2>Browse by category</h2>
             <div class="browsecats">
               <ul>
                 <li class="buildings"><a href=""><span class="icon"></span> Buildings &amp; Town plans</a></li>
                 <li class="maps"><a href=""><span class="icon"></span> Maps</a></li>
                 <li class="documents"><a href=""><span class="icon"></span> Documents</a></li>
                 <li class="images"><a href=""><span class="icon"></span> Images</a></li>
                 <li class="persons"><a href=""><span class="icon"></span> Persons</a></li>
                 <li class="organisations"><a href=""><span class="icon"></span> Organisations</a></li>
               </ul>
             </div>
           </div>';
  return $html;
}

function theme_delftdora_find_on_map_block(array &$variables){
  $path = drupal_get_path('module', 'delftdora');
  drupal_add_css("$path/css/delftdora.findonmap.css", array('type' => 'file'));
  $html = '<a href="" id="findonmap" title="Find on map">
             <span class="icon"></span><em>Find on map</em>
           </a>';
  return $html;
}

/**
 * Implements hook_preprocess_theme().
 */
function delftdora_preprocess_delftdora_facets_block_facet_field(array &$variables) {
  drupal_add_js(drupal_get_path('module', 'delftdora') . '/js/delftdora_solr.js', 'file');
  $variables['display_limit'] = variable_get('islandora_solr_facet_soft_limit', '10');
  drupal_add_js(array(
      'delftdoraSolr' => array(
        'displayLimit' => $variables['display_limit'],
      ),
    ), 'setting');
  $solr = $variables['solr'];
  $field = $variables['field'];
  $num_found = $solr->islandoraSolrResult['response']['numFound'];
  $options = array();
  foreach ($variables['options'] as $label => $count) {
    $option = delftdora_facets_block_facet_field_option($solr, $field, $label, $count);
    // Skip unselected facets that are unable to refine the results futher.
    if ($count == $num_found && !$option['selected']) {
      continue;
    }
    $options[] = $option;
  }
  list($label, $show_more_label) = delftdora_facets_block_facet_field_labels($variables['field']);
  $variables['label'] = $label;
  $variables['options'] = $options;
  $variables['show_more'] = count($options) >= $variables['display_limit'];
  $variables['show_text'] = t('All !label', array('!label' => strtolower($label)));
  $variables['hide_text'] = t('Hide !label', array('!label' => strtolower($show_more_label)));
}

/**
 * Get the labels to be used for the given field's label and show more label.
 *
 * @param string $field
 *   The name of the field to get the label for.
 *
 * @return array
 *   An associative array that contains two variables for use in the
 *   "delftdora_facets_block_facet_field" theme:
 *   - label: The label to display for this facet field.
 *   - show_more_label: The label to display for this facet field show more
 *     link.
 */
function delftdora_facets_block_facet_field_labels($field) {
  $settings = islandora_solr_get_fields('facet_fields', TRUE, FALSE, TRUE);
  $settings = _islandora_solr_simplify_fields($settings);
  $label = strtolower($settings[$field]);
  switch ($label) {
    case 'geography':
      $show_more_label = 'countries';
      break;

    case 'type':
      $show_more_label = 'types';
      break;

    case 'subject':
      $show_more_label = 'subjects';
      break;

    default:
      $show_more_label = $label;
  }
  return array(ucfirst($label), $show_more_label);
}

/**
 * Get the theme variables for the given facet field option.
 */
function delftdora_facets_block_facet_field_option($solr, $field, $label, $count) {
  $filter = $field . ':"' . $label . '"';
  $selected = delftdora_facets_block_facet_field_selected($solr, $filter);
  $url = $selected ?
    delftdora_facets_block_facet_field_remove_facet_url($solr, $filter) :
    delftdora_facets_block_facet_field_add_facet_url($solr, $filter);
  return array(
    'url' => $url,
    'selected' => $selected,
    'classes' => $selected ? array('selected') : array(),
    'label' => ucfirst($label),
    'count' => $count,
  );
}

/**
 * Check if the given filter being applied.
 */
function delftdora_facets_block_facet_field_selected($solr, $filter) {
  $fq = isset($solr->solrParams['fq']) ? $solr->solrParams['fq'] : array();
  return array_search($filter, $fq) !== FALSE;
}

/**
 * Generate a URL to add this filter to the Solr search results.
 */
function delftdora_facets_block_facet_field_add_facet_url($solr, $filter) {
  // Parameters set in URL.
  $params = $solr->internalSolrParams;
  // Set filter key if there are no filters included.
  if (!isset($params['f'])) {
    $params['f'] = array();
  }
  // Merge recursively to add new filter parameter.
  $params = array_merge_recursive($params, array('f' => array($filter)));
  // Current path including query, for example islandora/solr/query.
  // $_GET['q'] didn't seem to work here.
  $path = ISLANDORA_SOLR_SEARCH_PATH . '/' . trim(islandora_solr_replace_slashes($solr->solrQuery));
  return url($path, array('query' => $params));
}

/**
 * Generate a URL to remove this filter to the Solr search results.
 */
function delftdora_facets_block_facet_field_remove_facet_url($solr, $filter) {
  $remove_filter = function($f) use($filter) {
    return $f != $filter;
  };
  // Parameters set in URL.
  $params = $solr->internalSolrParams;
  // Set filter key if there are no filters included.
  if (!isset($params['f'])) {
    $params['f'] = array();
  }
  $params['f'] = array_filter($params['f'], $remove_filter);
  // Current path including query, for example islandora/solr/query.
  // $_GET['q'] didn't seem to work here.
  $path = ISLANDORA_SOLR_SEARCH_PATH . '/' . trim(islandora_solr_replace_slashes($solr->solrQuery));
  return url($path, array('query' => $params));
}
